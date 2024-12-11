#include "Player.h"
#include <iostream>

Player::Player(EventDispatcher& dispatcher)
    : m_frameWidth(192), m_frameHeight(192), m_elapsedTime(0.f),
    m_startFrame(0), m_endFrame(0), m_currentFrame(0), m_frameTime(0.1f), m_speed(200.f), m_state(AnimationState::Idle) {
    
    const char* spriteSheetPath = "resources/tiny_swords/Factions/Knights/Troops/Warrior/Blue/Warrior_Blue.png";

    if (!m_texture.loadFromFile(spriteSheetPath)) {
        std::cerr << "Failed to load sprite sheet: " << spriteSheetPath << std::endl;
    }

    m_texture.setSmooth(false);
    m_sprite.setTexture(m_texture);

    m_frameRect = sf::IntRect(0, 0, m_frameWidth, m_frameHeight);
    m_sprite.setTextureRect(m_frameRect);

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
    event.GetWindow().draw(m_sprite);
}


void Player::setPosition(float x, float y) {
    m_sprite.setPosition(x, y);
}

void Player::setAnimation(AnimationState animationState) {
    switch (animationState)
    {
    case AnimationState::Idle:
        m_startFrame = 0;
        m_endFrame = 5;
        m_frameTime = 0.1f;
        m_currentFrame = m_startFrame;
        break;
    case AnimationState::Walking:
        m_startFrame = 6;
        m_endFrame = 11;
        m_frameTime = 0.1f;
        m_currentFrame = m_startFrame;
        break;
    case AnimationState::SlashDown:
        m_startFrame = 12;
        m_endFrame = 17;
        m_frameTime = 0.1f;
        m_currentFrame = m_startFrame;
        break;
    case AnimationState::SlashUp:
        m_startFrame = 18;
        m_endFrame = 23;
        m_frameTime = 0.1f;
        m_currentFrame = m_startFrame;
        break;
    case AnimationState::SlashForwardLeft:
        m_startFrame = 24;
        m_endFrame = 29;
        m_frameTime = 0.1f;
        m_currentFrame = m_startFrame;
        break;
    case AnimationState::SlashForwardRight:
        m_startFrame = 30;
        m_endFrame = 35;
        m_frameTime = 0.1f;
        m_currentFrame = m_startFrame;
        break;
    case AnimationState::SlashBehindLeft:
        m_startFrame = 36;
        m_endFrame = 41;
        m_frameTime = 0.1f;
        m_currentFrame = m_startFrame;
        break;
    case AnimationState::SlashBehindRight:
        m_startFrame = 42;
        m_endFrame = 47;
        m_frameTime = 0.1f;
        m_currentFrame = m_startFrame;
        break;
    default:
        break;
    }

    m_state = animationState;
}

void Player::move(float deltaX, float deltaY) {
    m_sprite.move(deltaX, deltaY);
}

void Player::updatePlayer(float deltaTime) {
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
        if (deltaX == 0.f && deltaY == 0.f && m_state == AnimationState::Walking) {
            setAnimation(AnimationState::Idle);
    }

    move(deltaX, deltaY);
}


bool Player::isHitting() const  {
    return (m_state != AnimationState::Idle && m_state != AnimationState::Walking);
}




