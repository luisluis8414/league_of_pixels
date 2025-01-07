#pragma once
#include <SFML/Graphics.hpp>

struct AbilityLocationData {
    sf::Vector2f hitboxSize;
    sf::Vector2f mousePosition;
    sf::Vector2f hitboxOffset = { 0.f ,0.f };
    sf::Vector2f abilityOffset = { 0.f ,0.f };
};

class Ability {
public:
    Ability(sf::Texture& texture, int frameWidth, int frameHeight, int totalFrames,
        float frameDuration, int dmgFrame, AbilityLocationData locationData, float scale = 2.f)
        : m_sprite(texture), m_position(locationData.mousePosition), m_frameWidth(frameWidth), m_frameHeight(frameHeight),
        m_totalFrames(totalFrames), m_frameDuration(frameDuration), m_elapsedTime(0.f), m_currentFrame(0),
        m_isActive(true), m_dmgFrame(dmgFrame), m_dmg(10.f), m_hasEmitted(false)
    {
        // Set the sprite scale and position
        m_sprite.setScale(scale, scale);
        m_sprite.setPosition(locationData.mousePosition.x - (frameWidth / 2.f) * scale - locationData.abilityOffset.x,
            locationData.mousePosition.y - (frameHeight / 2.f) * scale - locationData.abilityOffset.y);

        float scaledWidth = locationData.hitboxSize.x * scale;
        float scaledHeight = locationData.hitboxSize.y * scale;
        m_hitbox = sf::FloatRect(
            locationData.mousePosition.x - (scaledWidth / 2.f) - locationData.hitboxOffset.x,
            locationData.mousePosition.y - (scaledHeight / 2.f) - locationData.hitboxOffset.y,
            scaledWidth,
            scaledHeight
        );
    }


    void update(float deltaTime) {
        if (!m_isActive) return;

        m_elapsedTime += deltaTime;

        if (m_elapsedTime >= m_frameDuration) {
            m_elapsedTime = 0.f;
            m_currentFrame++;
            if (m_currentFrame >= m_totalFrames) {
                m_isActive = false;
            }
        }

        m_sprite.setTextureRect(sf::IntRect(
            m_currentFrame * m_frameWidth, 0,
            m_frameWidth, m_frameHeight));
    }

    void draw(sf::RenderWindow& window) {
        if (m_isActive) {
            window.draw(m_sprite);
        }
    }

    bool isActive() const {
        return m_isActive;
    }

    const bool isInDmgFrame() const {
        return m_currentFrame == m_dmgFrame;
    }

    const bool hasEmitted() const {
        return m_hasEmitted;
    }

    void emit()  {
        m_hasEmitted = true;
    }

    const sf::FloatRect getHitbox() const {
        return m_hitbox;
    }

    const sf::Vector2f getPosition() const {
        return m_position;
    }

    const float getDmg() const {
        return m_dmg;
    }

private:
    sf::Sprite m_sprite;
    sf::Vector2f m_position;
    sf::FloatRect m_hitbox;
    int m_frameWidth;
    int m_frameHeight;
    int m_totalFrames;
    float m_frameDuration;
    float m_elapsedTime;
    int m_currentFrame;
    bool m_isActive;

    bool m_hasEmitted;
    int m_dmgFrame;
    float m_dmg;
};
