#pragma once

#include <SFML/Graphics.hpp>
#include "Event.h"

struct AnimationConfig {
    int startFrame;
    int endFrame;
    float frameTime; 
};


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

private:
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


    float m_maxHealth; // maximum health
    float m_currentHealth; // current health

    AnimationState m_state;// current animation state
    bool isHitting() const;     // flag to indicate if hit animation is active

    sf::RectangleShape m_healthBarBackground; // health bar background
    sf::RectangleShape m_healthBarForeground; // health bar foreground

    void updateHealthBar();
    void updateHitbox();

    void move(float deltaX, float deltaY); // move the player
    void updatePlayer(float deltaTime);

    void updateAnimation(float deltaTime); // update animation
    void onDraw(DrawEvent& event); // draw the player

    void setPosition(float x, float y); // set the player's position
    void setAnimation(AnimationState state);

    const std::unordered_map<AnimationState, AnimationConfig> m_animationConfigs = {
    {AnimationState::Idle, {0, 5, 0.1f}},
    {AnimationState::Walking, {6, 11, 0.1f}},
    {AnimationState::SlashDown, {12, 17, 0.1f}},
    {AnimationState::SlashUp, {18, 23, 0.1f}},
    {AnimationState::SlashForwardLeft, {24, 29, 0.1f}},
    {AnimationState::SlashForwardRight, {30, 35, 0.1f}},
    {AnimationState::SlashBehindLeft, {36, 41, 0.1f}},
    {AnimationState::SlashBehindRight, {42, 47, 0.1f}}
    };
};

