#include "Player.h"
#include <iostream>

Player::Player(EventDispatcher& dispatcher,float x, float y)
    : Entity(dispatcher, 192, 192, x, y, 0.1f, 200.f, 100.f, EntityType::Player), m_state(PlayerAnimationState::Idle) {
    const char* spriteSheetPath = "resources/tiny_swords/Factions/Knights/Troops/Warrior/Blue/Warrior_Blue.png";

    if (!m_texture.loadFromFile(spriteSheetPath)) {
        std::cerr << "Failed to load sprite sheet: " << spriteSheetPath << std::endl;
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
        this->onDraw(event);
    });

    dispatcher.subscribe<TickEvent>(this, [this](TickEvent& event) {
        this->onUpdate(event.GetDeltaTime());
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
    sf::RenderWindow& window = event.GetWindow();
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


void Player::setPosition(float x, float y) {
    m_sprite.setPosition(x, y);
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

void Player::move(float deltaX, float deltaY) {
    m_sprite.move(deltaX, deltaY);
    
    // flip sprite when walking in other direction
    if (deltaX < 0) {
        m_sprite.setScale(-1.f, 1.f);
        m_sprite.setOrigin((float)m_frameWidth, 0.f); 
    }
    else if (deltaX > 0) {
        m_sprite.setScale(1.f, 1.f); 
        m_sprite.setOrigin(0, 0); 
    }
}

void Player::onUpdate(const float deltaTime) {
    updateHealthBar();
    updateHitbox();
    updateAnimation(deltaTime);

    if (isHitting()) return;
    float deltaX = 0.f;
    float deltaY = 0.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            deltaY -= m_speed * deltaTime;
            if (m_state != PlayerAnimationState::Walking) {
                setAnimation(PlayerAnimationState::Walking);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            deltaY += m_speed * deltaTime;
            if (m_state != PlayerAnimationState::Walking) {
                setAnimation(PlayerAnimationState::Walking);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            deltaX -= m_speed * deltaTime;
            if (m_state != PlayerAnimationState::Walking) {
                setAnimation(PlayerAnimationState::Walking);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            deltaX += m_speed * deltaTime;
            if (m_state != PlayerAnimationState::Walking) {
                setAnimation(PlayerAnimationState::Walking);
            }
        }

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

        // if no movement keys are pressed, return to idle (only if not hitting)
        // the check in the animation loop isnt sufficient cause it only changes animation after finished
        if (deltaX == 0.f && deltaY == 0.f && m_state == PlayerAnimationState::Walking) {
            setAnimation(PlayerAnimationState::Idle);
    }

    move(deltaX, deltaY);

}

void Player::updateHealthBar() {
    //m_currentHealth--;
 /*   if (m_currentHealth <= 0) {
        DestroyEntityEvent destroyEvent(this);
        m_dispatcher.emit(destroyEvent);
        return;
    }*/
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

            m_attackHitbox = sf::FloatRect(attackHitboxLeft, attackHitboxTop, attackHitboxWidth, attackHitboxHeight);
        }
       
    }
    else {
        // reset when not attacking
        m_attackHitbox = sf::FloatRect(0.f, 0.f, 0.f, 0.f); 
    }
}


bool Player::isHitting() const  {
    return (m_state != PlayerAnimationState::Idle && m_state != PlayerAnimationState::Walking);
}




