#pragma once

#include <SFML/Graphics.hpp>
#include "Event.h"
#include "Entity.h"
#include <unordered_map>

enum class PlayerAnimationState {
    Idle,
    Walking,
    SlashDown,
    SlashUp,
    SlashForwardLeft,
    SlashForwardRight,
    SlashBehindLeft,
    SlashBehindRight
};


class Player : public Entity {
public:
    Player(EventDispatcher& dispatcher, sf::Vector2f position);

    ~Player() = default;

    const sf::FloatRect& getAttackHitbox() const {
        return m_attackHitbox;
    };

     bool isHitting() const;
private:
    PlayerAnimationState m_state;
    sf::FloatRect m_attackHitbox;
    //CollisionSystem& m_collisionSystem;

    void updateHealthBar() override;
    void updateHitbox() override;

    void onCollision() override;

    void move(float deltaTime) override;
    void updateAnimation(float deltaTime) override;

    void onUpdate(float deltaTime) override;
    void onDraw(DrawEvent& event) override;

    void setAnimation(PlayerAnimationState state);

    const std::unordered_map<PlayerAnimationState, AnimationConfig> m_animationConfigs = {
        {PlayerAnimationState::Idle, {0, 5, 0.1f}},
        {PlayerAnimationState::Walking, {6, 11, 0.1f}},
        {PlayerAnimationState::SlashDown, {12, 17, 0.1f}},
        {PlayerAnimationState::SlashUp, {18, 23, 0.1f}},
        {PlayerAnimationState::SlashForwardLeft, {24, 29, 0.1f}},
        {PlayerAnimationState::SlashForwardRight, {30, 35, 0.1f}},
        {PlayerAnimationState::SlashBehindLeft, {36, 41, 0.1f}},
        {PlayerAnimationState::SlashBehindRight, {42, 47, 0.1f}}
    };

    std::unordered_map<PlayerAnimationState, HitboxConfig> m_attackHitboxConfigs = {
    //{float widthFactor; heightFactor; offsetXFactor; offsetYFactor; }
    {PlayerAnimationState::SlashDown, {0.35f, 0.5f, 0.6f, 0.2f}},
    {PlayerAnimationState::SlashUp, {0.6f, 0.25f, 0.4f, 0.4f}},
    {PlayerAnimationState::SlashForwardRight, {0.6f, 0.3f, 0.3f, 0.55f}},
    {PlayerAnimationState::SlashBehindLeft, {0.6f, 0.25f, 0.2f, 0.2f}},
    };
};
