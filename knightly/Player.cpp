#include "Player.h"
#include "CollisionSystem.h"
#include "Map.h"

Player::Player(EventDispatcher& dispatcher, sf::Vector2f position)
    : Entity(dispatcher, 192, 192, position, 0.1f, 200.f, 100.f, EntityType::Player, "resources/tiny_swords/Factions/Knights/Troops/Warrior/Blue/Warrior_Blue.png"), m_state(PlayerAnimationState::Idle) {
       
    m_dispatcher.subscribe<DrawEvent>(this, [this](DrawEvent& event) {
        onDraw(event);
    }, RenderLayer::PLAYER);

    m_dispatcher.subscribe<TickEvent>(this, [this](TickEvent& event) {
        onUpdate(event.getDeltaTime());
    });

    dispatcher.subscribe<MouseRightClickEvent>(this, [this](MouseRightClickEvent event) {
        setDestination(event.getPosition());
        });
}


void Player::updateAnimation(float deltaTime) {
    m_elapsedTime += deltaTime;

    if (m_elapsedTime >= m_frameTime) {
        m_elapsedTime = 0.f;

        m_currentFrame++;
        if (m_currentFrame > m_endFrame) {
            if (m_state != PlayerAnimationState::Idle) {
                setAnimation(PlayerAnimationState::Idle);
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

void Player::onDraw(DrawEvent& event) {
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

    //sprite border
    sf::FloatRect bounds = m_sprite.getGlobalBounds();

    sf::RectangleShape border;
    border.setPosition(bounds.left, bounds.top); // Top-left corner of the sprite
    border.setSize({ bounds.width, bounds.height }); // Size of the sprite
    border.setFillColor(sf::Color::Transparent); // Make the inside transparent
    border.setOutlineColor(sf::Color::Red); // Set the border color
    border.setOutlineThickness(1.f); // Set the border thickness

    window.draw(border);
}


void Player::setAnimation(PlayerAnimationState animationState) {
    if (m_animationConfigs.count(animationState) > 0) {
        const auto& config = m_animationConfigs.at(animationState);
        m_startFrame = config.startFrame;
        m_endFrame = config.endFrame;
        m_frameTime = config.frameTime;
        m_currentFrame = m_startFrame;
    }
    m_state = animationState;
}

void Player::move(float deltaTime) {
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

        sf::FloatRect newXHitbox = m_hitbox;
        newXHitbox.left += deltaX;


        sf::FloatRect newYHitbox = m_hitbox;
        newYHitbox.top += deltaY;

        bool canMoveX = Map::isTilePassable(newXHitbox.left, newXHitbox.top) && Map::isTilePassable((newXHitbox.left + newXHitbox.width), newXHitbox.top);
        bool canMoveY = Map::isTilePassable(newYHitbox.left, newYHitbox.top) && Map::isTilePassable(newYHitbox.left, (newYHitbox.top + newYHitbox.height));

        if (canMoveX) {
            m_sprite.move(deltaX, 0.f);
        }

        if (canMoveY) {
            m_sprite.move(0.f, deltaY);
        }

        if (m_state != PlayerAnimationState::Walking) {
            setAnimation(PlayerAnimationState::Walking);
        }

        if ((!canMoveX && deltaY == 0) || (!canMoveY && deltaX== 0)) {
            m_destination = m_sprite.getPosition();
        }
    }
    else {
        if (m_state != PlayerAnimationState::Idle) {
            setAnimation(PlayerAnimationState::Idle);
        }
    }
}

void Player::onUpdate(const float deltaTime) {
    updateHealthBar();
    updateHitbox();
    updateAnimation(deltaTime);

    if (isHitting()) return;

    move(deltaTime);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        setAnimation(PlayerAnimationState::SlashDown);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        setAnimation(PlayerAnimationState::SlashUp);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        setAnimation(PlayerAnimationState::SlashForwardRight);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        setAnimation(PlayerAnimationState::SlashBehindLeft);
    }
}

void Player::updateHealthBar() {
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

void Player::updateHitbox() {
    sf::FloatRect spriteBounds = m_sprite.getGlobalBounds();

    float hitboxWidth = spriteBounds.width * 0.3f;
    float hitboxHeight = spriteBounds.height * 0.4f;

    float hitboxLeft = spriteBounds.left + (spriteBounds.width - hitboxWidth) / 2.f;
    float hitboxTop = spriteBounds.top + (spriteBounds.height - hitboxHeight) / 2.f;

    m_hitbox = sf::FloatRect(hitboxLeft, hitboxTop, hitboxWidth, hitboxHeight);

    // attack hitbox based on animation state
    if (isHitting() && m_attackHitboxConfigs.count(m_state) > 0) {
        const auto& config = m_attackHitboxConfigs.at(m_state);

        // check if current frame is within the last two frames of the animation
        if (m_currentFrame >= (m_endFrame - 2)) {
            float attackHitboxWidth = spriteBounds.width * config.widthFactor;
            float attackHitboxHeight = spriteBounds.height * config.heightFactor;

            float attackHitboxLeft = spriteBounds.left + spriteBounds.width * config.offsetXFactor;
            float attackHitboxTop = spriteBounds.top + spriteBounds.height * config.offsetYFactor;

            // flip the attack hitbox if facing left
            float direction = (m_sprite.getScale().x < 0.f) ? -1.f : 1.f;
            if (direction < 0.f) {
                float centerX = spriteBounds.left + spriteBounds.width * 0.5f;
                float offsetFromCenter = attackHitboxLeft - centerX;
                attackHitboxLeft = centerX - offsetFromCenter - attackHitboxWidth;
            }

            m_attackHitbox = sf::FloatRect(attackHitboxLeft, attackHitboxTop, attackHitboxWidth, attackHitboxHeight);
        }

    }
    else {
        // reset when not attacking
        m_attackHitbox = sf::FloatRect(0.f, 0.f, 0.f, 0.f);
    }
}

void Player::onCollision() {
    m_currentHealth -= 0.1f;
}

bool Player::isHitting() const  {
    return (m_state != PlayerAnimationState::Idle && m_state != PlayerAnimationState::Walking);
}


