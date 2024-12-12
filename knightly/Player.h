#pragma once

#include <SFML/Graphics.hpp>
#include "Event.h"
#include "Entity.h"
#include <unordered_map>

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

struct HitboxConfig {
    float widthFactor;
    float heightFactor;
    float offsetXFactor;
    float offsetYFactor;
};

class Player : public Entity {
public:
    explicit Player(EventDispatcher& dispatcher);

    ~Player() = default;
private:
    AnimationState m_state; 
    sf::FloatRect m_attackHitbox;

    bool isHitting() const;

    void updateHealthBar() override;
    void updateHitbox() override;

    void move(float deltaX, float deltaY) override;
    void updateAnimation(float deltaTime) override;
    void setPosition(float x, float y) override;

    void onUpdate(float deltaTime) override;
    void onDraw(DrawEvent& event) override;

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

    std::unordered_map<AnimationState, HitboxConfig> m_attackHitboxConfigs = {
    //{float widthFactor; heightFactor; offsetXFactor; offsetYFactor; }
    {AnimationState::SlashDown, {0.35f, 0.5f, 0.6f, 0.2f}}, 
    {AnimationState::SlashUp, {0.6f, 0.25f, 0.4f, 0.4f}},
    {AnimationState::SlashForwardRight, {0.6f, 0.3f, 0.3f, 0.55f}}, 
    {AnimationState::SlashBehindLeft, {0.6f, 0.25f, 0.2f, 0.2f}},  
    };
};
