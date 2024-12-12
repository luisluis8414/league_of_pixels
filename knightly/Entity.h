#pragma once

#include <SFML/Graphics.hpp>
#include "Event.h"

struct AnimationConfig {
    int startFrame;
    int endFrame;
    float frameTime;
};

class Entity {
public:
    explicit Entity(
        EventDispatcher& dispatcher,
        int frameWidth,
        int frameHeight,
        float frameTime,
        float speed,
        float maxHealth
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
        m_currentHealth(maxHealth) {
    }

    virtual ~Entity() = default;  

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
};
