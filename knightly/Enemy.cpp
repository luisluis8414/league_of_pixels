    #include "Enemy.h"
    #include <iostream>

    Enemy::Enemy(EventDispatcher& dispatcher, const std::string& texturePath, float x,float y)
        : Entity(dispatcher, 192, 192, x, y, 0.1f, 200.f, 100.f, EntityType::Enemy), m_state(EnemyAnimationState::Idle) {

        if (!m_texture.loadFromFile(texturePath)) {
            std::cerr << "Failed to load sprite sheet: " << texturePath << std::endl;
        }

        m_texture.setSmooth(false);
        m_sprite.setTexture(m_texture);

        m_frameRect = sf::IntRect(0, 0, m_frameWidth, m_frameHeight);
        m_sprite.setTextureRect(m_frameRect);

        m_healthBarBackground.setSize(sf::Vector2f(100.f, 10.f));
        m_healthBarBackground.setFillColor(sf::Color::Red);
        m_healthBarBackground.setPosition(10.f, 10.f);

        m_healthBarForeground.setSize(sf::Vector2f(100.f, 10.f));
        m_healthBarForeground.setFillColor(sf::Color::Green);
        m_healthBarForeground.setPosition(10.f, 10.f);

        dispatcher.subscribe<DrawEvent>(this, [this](DrawEvent& event) {
            onDraw(event);
            }, RenderLayer::ENTITIES);

        dispatcher.subscribe<TickEvent>(this, [this](TickEvent& event) {
            onUpdate(event.getDeltaTime());
            });

        dispatcher.subscribe<CollisionEvent>(this, [this](CollisionEvent& event) {
            const Entity& entityA = event.getEntityA();
            const Entity& entityB = event.getEntityB();

            if (&entityA == this || &entityB == this) {
            onCollision();
            }
        });
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
            m_frameRect = sf::IntRect(column * m_frameWidth, row * m_frameHeight, m_frameWidth, m_frameHeight);
            m_sprite.setTextureRect(m_frameRect);
        }
    }

    void Enemy::onDraw(DrawEvent& event) {
        sf::RenderWindow& window = event.getWindow();
        window.draw(m_sprite);

        window.draw(m_healthBarBackground);
        window.draw(m_healthBarForeground);

        sf::RectangleShape hitboxShape;
        hitboxShape.setPosition(m_hitbox.left, m_hitbox.top);
        hitboxShape.setSize(sf::Vector2f(m_hitbox.width, m_hitbox.height));
        hitboxShape.setFillColor(sf::Color::Transparent);
        hitboxShape.setOutlineColor(sf::Color::Red);
        hitboxShape.setOutlineThickness(1.f);

        window.draw(hitboxShape);

        if (isHitting()) {
            sf::RectangleShape attackHitboxShape;
            attackHitboxShape.setPosition(m_attackHitbox.left, m_attackHitbox.top);
            attackHitboxShape.setSize(sf::Vector2f(m_attackHitbox.width, m_attackHitbox.height));
            attackHitboxShape.setFillColor(sf::Color::Transparent);
            attackHitboxShape.setOutlineColor(sf::Color::Green);
            attackHitboxShape.setOutlineThickness(1.f);
            window.draw(attackHitboxShape);
        }
    }


    void Enemy::setPosition(float x, float y) {
        m_sprite.setPosition(x, y);
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

    void Enemy::move(float deltaX, float deltaY) {
        if (deltaX != 0.f || deltaY != 0.f) {
            if (m_state != EnemyAnimationState::Walking) {
                setAnimation(EnemyAnimationState::Walking);
            }
        }
      /*  else if (m_state == EnemyAnimationState::Walking) {
            setAnimation(EnemyAnimationState::Idle);
        }*/
        m_sprite.move(deltaX, deltaY);
        MoveEvent moveEvent(m_sprite, deltaX, deltaY);
    }

    void Enemy::onUpdate(const float deltaTime) {
        updateHealthBar();
        updateHitbox();
        updateAnimation(deltaTime);

        if (isHitting()) return;
        float deltaX = 1.f;
        float deltaY = 0.f;

        if (deltaX == 0.f && deltaY == 0.f && m_state == EnemyAnimationState::Walking) {
            setAnimation(EnemyAnimationState::Idle);
        }

        move(deltaX, deltaY);
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
        float healthBarX = bounds.left + (bounds.width / 2.f) - (m_healthBarBackground.getSize().x / 2.f);
        float healthBarY = bounds.top - m_healthBarBackground.getSize().y + 25.f; // offset for spacing from top
        m_healthBarBackground.setPosition(healthBarX, healthBarY);
        m_healthBarForeground.setPosition(healthBarX, healthBarY);
    }

    void Enemy::updateHitbox() {
        sf::FloatRect spriteBounds = m_sprite.getGlobalBounds();

        float hitboxWidth = spriteBounds.width * 0.3f;
        float hitboxHeight = spriteBounds.height * 0.4f;

        float hitboxLeft = spriteBounds.left + (spriteBounds.width - hitboxWidth) / 2.f;
        float hitboxTop = spriteBounds.top + (spriteBounds.height - hitboxHeight) / 2.f;

        m_hitbox = sf::FloatRect(hitboxLeft, hitboxTop, hitboxWidth, hitboxHeight);
    }


    bool Enemy::isHitting() const {
        return m_state == EnemyAnimationState::Hitting;
    }

    void Enemy::onCollision() {
        m_currentHealth--;
    }

