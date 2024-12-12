#pragma once

#include <SFML/Graphics.hpp>
#include "Event.h"
#include "Entity.h"
#include <unordered_map>

enum class EnemyAnimationState {
    Idle,
    Walking,
};


class Enemy : public Entity {
public:
    explicit Enemy(EventDispatcher& dispatcher, const std::string& texturePath, float x, float y);

    ~Enemy() = default;
private:
    EnemyAnimationState m_state;
    sf::FloatRect m_attackHitbox;

    bool isHitting() const;

    void updateHealthBar() override;
    void updateHitbox() override;

    void move(float deltaX, float deltaY) override;
    void updateAnimation(float deltaTime) override;
    void setPosition(float x, float y) override;

    void onUpdate(float deltaTime) override;
    void onDraw(DrawEvent& event) override;

    void setAnimation(EnemyAnimationState state);

    const std::unordered_map<EnemyAnimationState, AnimationConfig> m_animationConfigs = {
        {EnemyAnimationState::Idle, {0, 5, 0.1f}},
        {EnemyAnimationState::Walking, {6, 11, 0.1f}},
    };

    //std::unordered_map<EnemyAnimationState, HitboxConfig> m_attackHitboxConfigs = {
    //    //{float widthFactor; heightFactor; offsetXFactor; offsetYFactor; }
    //    {PlayerAnimationState::SlashDown, {0.35f, 0.5f, 0.6f, 0.2f}},
    //    {PlayerAnimationState::SlashUp, {0.6f, 0.25f, 0.4f, 0.4f}},
    //    {PlayerAnimationState::SlashForwardRight, {0.6f, 0.3f, 0.3f, 0.55f}},
    //    {PlayerAnimationState::SlashBehindLeft, {0.6f, 0.25f, 0.2f, 0.2f}},
    //};
};
