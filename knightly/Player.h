#pragma once

#include <SFML/Graphics.hpp>
#include "Event.h"

enum class AnimationState {
    Idle,
    Walking,
    SlashDown,
    SlashUp,
    SlashForwardLeft,
    SlashForwardRight,
    SlashBehindLeft,
    SlashBehindRight
};

class Player {
public:
    Player(EventDispatcher& dispatcher);

    void update(float deltaTime); // update animation
    void onDraw(DrawEvent& event); // draw the player

    void setPosition(float x, float y); // set the player's position
    void setAnimation(AnimationState state); 

    void move(float deltaX, float deltaY); // move the player
    void handleInput(float deltaTime);    // handle WASD input

private:
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

    AnimationState m_state;    // current animation state
    //bool m_isHitAnimating;     // flag to indicate if hit animation is active
};

