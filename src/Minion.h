#pragma once

#include <SFML/Graphics.hpp>
#include "Event.h"
#include "Entity.h"
#include <unordered_map>

enum class MinionAnimationState {
    Walking,
    Hitting,
    Dying
};


class Minion : public Entity {
public:
    Minion(EventDispatcher& dispatcher, const std::string& texturePath, sf::Vector2f position, sf::Vector2f destination);

    ~Minion() = default;

    const sf::FloatRect& getAttackHitbox() const {
        return m_attackHitbox;
    };

    bool isHitting() const;
private:
    MinionAnimationState m_state;
    sf::FloatRect m_attackHitbox;

    void updateHealthBar() override;
    void updateHitbox() override;

    void onCollision() override;

    void move(float deltaTime) override;
    void updateAnimation(float deltaTime) override;

    void onUpdate(float deltaTime) override;
    void onDraw(DrawEvent& event) override;

    void setAnimation(MinionAnimationState state);

    const std::unordered_map<MinionAnimationState, AnimationConfig> m_animationConfigs = {
        {MinionAnimationState::Walking, {0, 5, 0.1f}},
        {MinionAnimationState::Hitting, {6, 15, 0.1f}},
        {MinionAnimationState::Dying, {16, 22, 0.1f}},
    };

    std::unordered_map<MinionAnimationState, HitboxConfig> m_attackHitboxConfigs = {
        //{float widthFactor; heightFactor; offsetXFactor; offsetYFactor; }
        {MinionAnimationState::Hitting, {0.35f, 0.5f, 0.6f, 0.2f}},
    };
};
