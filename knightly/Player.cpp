#include "Player.h"
#include <iostream>

Player::Player(EventDispatcher& dispatcher)
    : m_dispatcher(dispatcher), m_frameWidth(192), m_frameHeight(192), m_elapsedTime(0.f),
    m_startFrame(0), m_endFrame(5), m_currentFrame(0), m_frameTime(0.1f), m_speed(200.f), m_state(AnimationState::Idle), m_maxHealth(100.f), m_currentHealth(m_maxHealth) {
    
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

    dispatcher.subscribe<DrawEvent>([this](DrawEvent& event) {
        this->onDraw(event);
    });

    dispatcher.subscribe<TickEvent>([this](TickEvent& event) {
        this->updatePlayer(event.GetDeltaTime());
        this->updateAnimation(event.GetDeltaTime());
    });
}

void Player::updateAnimation(float deltaTime) {
    m_elapsedTime += deltaTime;

    if (m_elapsedTime >= m_frameTime) {
        m_elapsedTime = 0.f;

        m_currentFrame++;
        if (m_currentFrame > m_endFrame) {
            if (m_state != AnimationState::Idle) {
                setAnimation(AnimationState::Idle);
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
}


void Player::setPosition(float x, float y) {
    m_sprite.setPosition(x, y);
}

void Player::setAnimation(AnimationState animationState) {
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
}

void Player::updatePlayer(const float deltaTime) {
    updateHealthBar();
    updateHitbox();

    if (isHitting()) return;
    float deltaX = 0.f;
    float deltaY = 0.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            deltaY -= m_speed * deltaTime;
            if (m_state != AnimationState::Walking) {
                setAnimation(AnimationState::Walking);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            deltaY += m_speed * deltaTime;
            if (m_state != AnimationState::Walking) {
                setAnimation(AnimationState::Walking);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            deltaX -= m_speed * deltaTime;
            if (m_state != AnimationState::Walking) {
                setAnimation(AnimationState::Walking);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            deltaX += m_speed * deltaTime;
            if (m_state != AnimationState::Walking) {
                setAnimation(AnimationState::Walking);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            setAnimation(AnimationState::SlashDown);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            setAnimation(AnimationState::SlashUp);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            setAnimation(AnimationState::SlashForwardRight);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            setAnimation(AnimationState::SlashBehindLeft);
        }

        // if no movement keys are pressed, return to idle (only if not hitting)
        // the check in the animation loop isnt sufficient cause it only changes animation after finished
        if (deltaX == 0.f && deltaY == 0.f && m_state == AnimationState::Walking) {
            setAnimation(AnimationState::Idle);
    }

    move(deltaX, deltaY);
}

void Player::updateHealthBar() {
    if (m_currentHealth <= 0) {
        GameOverEvent gameOver;
        m_dispatcher.emit(gameOver);
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
}


bool Player::isHitting() const  {
    return (m_state != AnimationState::Idle && m_state != AnimationState::Walking);
}




