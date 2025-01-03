#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Event.h"
#include <random>

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
        float x_posi,
        float y_posi,
        float frameTime,
        float speed,
        float maxHealth,
        EntityType type
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
        m_type(type)
    {
        m_sprite.setPosition(x_posi, y_posi);
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

    float m_speed;             // movement speed

    float m_maxHealth;         // maximum health
    float m_currentHealth;     // current health


    sf::RectangleShape m_healthBarBackground; // health bar background
    sf::RectangleShape m_healthBarForeground; // health bar foreground

    virtual void updateHealthBar() = 0;
    virtual void updateHitbox() = 0;

    virtual void move(float deltaX, float deltaY) = 0; 
    virtual void updateAnimation(float deltaTime) = 0; 
    virtual void setPosition(float x, float y) = 0; 

    virtual void onUpdate(float deltaTime) = 0; 
    virtual void onDraw(DrawEvent& event) = 0; 

    virtual void onCollision() = 0;

    EntityType m_type;
};
