#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Event.h"
#include <iostream>

enum class EntityType {
    Player,
    Enemy,
};

struct AnimationConfig {
    int startFrame;
    int endFrame;
    float frameTime;
};

struct HitboxConfig {
    float widthFactor;
    float heightFactor;
    float offsetXFactor;
    float offsetYFactor;
};

//inline uint64_t generateRandomId() {
//    static std::random_device rd;
//    static std::mt19937_64 gen(rd());
//    static std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);
//    return dist(gen);
//}

class Entity {
public:
    Entity(
        EventDispatcher& dispatcher,
        int frameWidth,
        int frameHeight,
        sf::Vector2f position,
        float frameTime,
        float speed,
        float maxHealth,
        EntityType type,
        std::string texturePath
    )
        : m_dispatcher(dispatcher),
        m_frameWidth(frameWidth),
        m_frameHeight(frameHeight),
        m_elapsedTime(0.f),
        m_startFrame(0),
        m_endFrame(5),
        m_currentFrame(0),
        m_frameTime(frameTime),
        m_speed(speed),
        m_maxHealth(maxHealth),
        m_currentHealth(maxHealth),
        m_type(type),
        m_destination(position)
    {
        if (!m_texture.loadFromFile(texturePath)) {
            std::cerr << "Failed to load sprite sheet: " << texturePath << std::endl;
        }

        m_texture.setSmooth(false);
        m_sprite.setTexture(m_texture);

        m_frameRect = sf::IntRect(0, 0, m_frameWidth, m_frameHeight);
        m_sprite.setTextureRect(m_frameRect);

        m_sprite.setPosition(position.x, position.y);

        m_sprite.setOrigin(m_sprite.getGlobalBounds().width / 2.f, m_sprite.getGlobalBounds().height / 2.f);

        m_healthBarBackground.setSize(sf::Vector2f(100.f, 10.f));
        m_healthBarBackground.setFillColor(sf::Color::Red);
        m_healthBarBackground.setPosition(10.f, 10.f);

        m_healthBarForeground.setSize(sf::Vector2f(100.f, 10.f));
        m_healthBarForeground.setFillColor(sf::Color::Green);
        m_healthBarForeground.setPosition(10.f, 10.f);
    }


    ~Entity() {
        m_dispatcher.unsubscribe(this);
    }

    EntityType getType() const { return m_type; }
    const sf::FloatRect& getHitbox() const { return m_hitbox; }
protected:
    EventDispatcher& m_dispatcher;

    sf::FloatRect m_hitbox;

    sf::Texture m_texture;     // the sprite sheet
    sf::Sprite m_sprite;       // the player's sprite
    sf::IntRect m_frameRect;   // rectangle defining the current frame

    int m_startFrame;          // start of the animation
    int m_endFrame;            // end of the animation
    int m_currentFrame;        // current frame index
    float m_frameTime;         // time per frame
    float m_elapsedTime;       // time elapsed since last frame change

    int m_frameWidth;          // width of one frame
    int m_frameHeight;         // height of one frame

    sf::Vector2f m_destination;
    float m_speed;             // movement speed

    float m_maxHealth;         // maximum health
    float m_currentHealth;     // current health


    sf::RectangleShape m_healthBarBackground; // health bar background
    sf::RectangleShape m_healthBarForeground; // health bar foreground

    virtual void updateHealthBar() = 0;
    virtual void updateHitbox() = 0;

    virtual void move(float deltaTime) = 0; 
    virtual void updateAnimation(float deltaTime) = 0; 

    virtual void setPosition(sf::Vector2f position) {
        m_sprite.setPosition(position.x, position.y);
    };

    virtual void onUpdate(float deltaTime) = 0; 
    virtual void onDraw(DrawEvent& event) = 0; 

    virtual void onCollision() = 0;

    void setDestination(sf::Vector2f position) {
        sf::Vector2f direction = position - m_sprite.getPosition();

        constexpr float flipThreshold = 1.f;

        if (direction.x < -flipThreshold && m_sprite.getScale().x != -1.f) {
            m_sprite.setScale(-1.f, 1.f); 
        }
        else if (direction.x > flipThreshold && m_sprite.getScale().x != 1.f) {
            m_sprite.setScale(1.f, 1.f); 
        }

        m_destination = position;
    }

    void clearDestination() {
        m_destination = m_sprite.getPosition();
    }

    sf::Vector2f getSpriteCenter() {
        sf::Vector2f currentPosition = m_sprite.getPosition();

        sf::FloatRect bounds = m_sprite.getGlobalBounds();

        return sf::Vector2f(
            currentPosition.x + bounds.width / 2.f,
            currentPosition.y + bounds.height / 2.f
        );
    }

    EntityType m_type;
};
