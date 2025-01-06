#pragma once
#include <SFML/Graphics.hpp>

class Ability {
public:
    Ability(sf::Texture& texture, sf::Vector2f position, int frameWidth, int frameHeight, int totalFrames, float frameDuration, float scale = 1.f)
        : m_sprite(texture), m_position(position), m_frameWidth(frameWidth), m_frameHeight(frameHeight),
        m_totalFrames(totalFrames), m_frameDuration(frameDuration), m_elapsedTime(0.f), m_currentFrame(0), m_isActive(true)
    {
        m_sprite.setScale(scale, scale);
        m_sprite.setPosition(position.x - (frameWidth / 2.f) * scale,
            position.y - (frameHeight / 2.f) * scale);

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

private:
    sf::Sprite m_sprite;
    sf::Vector2f m_position;
    int m_frameWidth;
    int m_frameHeight;
    int m_totalFrames;
    float m_frameDuration;
    float m_elapsedTime;
    int m_currentFrame;
    bool m_isActive;
};
