    #include "Enemy.h"
    #include <iostream>

    Enemy::Enemy(EventDispatcher& dispatcher, const std::string& texturePath, sf::Vector2f position)
        : Entity(dispatcher, 192, 192, position, 0.1f, 200.f, 1000.f, 10.f, EntityType::Enemy, texturePath), m_state(EnemyAnimationState::Idle) {
        m_dispatcher.subscribe<DrawEvent>(this, [this](DrawEvent& event) {
            onDraw(event);
            }, RenderLayer::ENTITIES);

        m_dispatcher.subscribe<TickEvent>(this, [this](TickEvent& event) {
            onUpdate(event.getDeltaTime());
            });

        m_dispatcher.subscribe<CollisionEvent>(this, [this](CollisionEvent& event) {
            const Entity& entityA = event.getEntityA();
            const Entity& entityB = event.getEntityB();

            if (&entityA == this || &entityB == this) {
                onCollision();
            }
        });
        //let them walk
        m_destination = { 1800.f, 200.f };
    }


    void Enemy::updateAnimation(float deltaTime) {
        m_elapsedTime += deltaTime;

        if (m_elapsedTime >= m_frameTime) {
            m_elapsedTime = 0.f;

            m_currentFrame++;
            if (m_currentFrame > m_endFrame) {
                if (m_state != EnemyAnimationState::Idle) {
                    setAnimation(EnemyAnimationState::Idle);
                }
                else {
                    m_currentFrame = m_startFrame;
                }
            }

            int column = m_currentFrame % (m_texture.getSize().x / m_frameWidth);
            int row = m_currentFrame / (m_texture.getSize().x / m_frameWidth);
            m_frameRect = sf::IntRect({ column * m_frameWidth, row * m_frameHeight }, { m_frameWidth, m_frameHeight });
            m_sprite.setTextureRect(m_frameRect);
        }
    }

    void Enemy::onDraw(DrawEvent& event) {
        sf::RenderWindow& window = event.getWindow();
        window.draw(m_sprite);

        window.draw(m_healthBarBackground);
        window.draw(m_healthBarForeground);

        sf::RectangleShape hitboxShape;
        hitboxShape.setPosition({ m_hitbox.position.x, m_hitbox.position.y });
        hitboxShape.setSize(sf::Vector2f(m_hitbox.size.x, m_hitbox.size.y));
        hitboxShape.setFillColor(sf::Color::Transparent);
        hitboxShape.setOutlineColor(sf::Color::Red);
        hitboxShape.setOutlineThickness(1.f);

        window.draw(hitboxShape);

        if (isHitting()) {
            sf::RectangleShape attackHitboxShape;
            attackHitboxShape.setPosition({ m_attackHitbox.position.x, m_attackHitbox.position.y });
            attackHitboxShape.setSize(sf::Vector2f(m_attackHitbox.size.x, m_attackHitbox.size.y));
            attackHitboxShape.setFillColor(sf::Color::Transparent);
            attackHitboxShape.setOutlineColor(sf::Color::Green);
            attackHitboxShape.setOutlineThickness(1.f);
            window.draw(attackHitboxShape);
        }
    }

    void Enemy::setAnimation(EnemyAnimationState animationState) {
        if (m_animationConfigs.count(animationState) > 0) {
            const auto& config = m_animationConfigs.at(animationState);
            m_startFrame = config.startFrame;
            m_endFrame = config.endFrame;
            m_frameTime = config.frameTime;
            m_currentFrame = m_startFrame;
        }
        m_state = animationState;
    }

    void Enemy::move(float deltaTime) {
        sf::Vector2f direction = m_destination - m_sprite.getPosition();

        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        constexpr float epsilon = 2.f;

        if (distance > epsilon) {
            direction /= distance;

            constexpr float minDirectionThreshold = 0.01f;
            if (std::abs(direction.x) < minDirectionThreshold) direction.x = 0.0f;
            if (std::abs(direction.y) < minDirectionThreshold) direction.y = 0.0f;

            float deltaX = direction.x * m_speed * deltaTime;
            float deltaY = direction.y * m_speed * deltaTime;

            MoveEvent moveEvent(
                m_sprite,
                m_hitbox,
                { deltaX, deltaY },
                m_destination
            );

            m_dispatcher.emit(moveEvent);

            if (m_state != EnemyAnimationState::Walking) {
                setAnimation(EnemyAnimationState::Walking);
            }
        }
        else {
            if (m_state != EnemyAnimationState::Idle) {
                setAnimation(EnemyAnimationState::Idle);
            }
        }
    }

    void Enemy::onUpdate(const float deltaTime) {
        updateHealthBar();
        updateHitbox();
        updateAnimation(deltaTime);

        if (isHitting()) return;

        move(deltaTime);
    }

    void Enemy::updateHealthBar() {
        if (m_currentHealth <= 0) {
            DestroyEntityEvent destroyEvent(this);
            m_dispatcher.emit(destroyEvent);
            return; 
        }
        float healthPercentage = m_currentHealth / m_maxHealth;
        m_healthBarForeground.setSize(sf::Vector2f(healthPercentage * 100.f, 10.f));

        sf::FloatRect bounds = m_sprite.getGlobalBounds();
        float healthBarX = bounds.position.x + (bounds.size.x / 2.f) - (m_healthBarBackground.getSize().x / 2.f);
        float healthBarY = bounds.position.y - m_healthBarBackground.getSize().y + 25.f; // offset for spacing from top
        m_healthBarBackground.setPosition({ healthBarX, healthBarY });
        m_healthBarForeground.setPosition({ healthBarX, healthBarY });
    }

    void Enemy::updateHitbox() {
        sf::FloatRect spriteBounds = m_sprite.getGlobalBounds();

        float hitboxWidth = spriteBounds.size.x * 0.3f;
        float hitboxHeight = spriteBounds.size.y * 0.4f;

        float hitboxLeft = spriteBounds.position.x + (spriteBounds.size.x - hitboxWidth) / 2.f;
        float hitboxTop = spriteBounds.position.y + (spriteBounds.size.y - hitboxHeight) / 2.f;

        m_hitbox = sf::FloatRect({ hitboxLeft, hitboxTop }, { hitboxWidth, hitboxHeight });
    }


    bool Enemy::isHitting() const {
        return m_state == EnemyAnimationState::Hitting;
    }

    void Enemy::onCollision() {
        m_currentHealth--;
    }

