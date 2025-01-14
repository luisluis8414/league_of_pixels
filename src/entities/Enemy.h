#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "../core/Event.h"
#include "Entity.h"

enum class EnemyAnimationState { IDLE, WALKING, Hitting };

class Enemy : public Entity {
 public:
  Enemy(EventDispatcher& dispatcher, const std::string& texturePath, sf::Vector2f position);

  ~Enemy() = default;

 private:
  EnemyAnimationState m_state;
  sf::FloatRect m_attackHitbox;

  bool isHitting() const;

  void updateHealthBar() override;
  void updateHitbox() override;

  void setWalking() override;
  void setIdle() override;

  void onAnimationEnd() override;

  void onUpdate(float deltaTime) override;
  void onDraw(DrawEvent& event) override;

  void setAnimation(EnemyAnimationState state);

  const std::unordered_map<EnemyAnimationState, AnimationConfig> m_animationConfigs = {
      {EnemyAnimationState::IDLE, {0, 5, 0.1f}},
      {EnemyAnimationState::WALKING, {6, 11, 0.1f}},
      {EnemyAnimationState::Hitting, {6, 11, 0.1f}},
  };

  // std::unordered_map<EnemyAnimationState, HitboxConfig> m_attackHitboxConfigs = {
  //     //{float widthFactor; heightFactor; offsetXFactor; offsetYFactor; }
  //     {PlayerAnimationState::SlashDown, {0.35f, 0.5f, 0.6f, 0.2f}},
  //     {PlayerAnimationState::SlashUp, {0.6f, 0.25f, 0.4f, 0.4f}},
  //     {PlayerAnimationState::SlashForwardRight, {0.6f, 0.3f, 0.3f, 0.55f}},
  //     {PlayerAnimationState::SlashBehindLeft, {0.6f, 0.25f, 0.2f, 0.2f}},
  // };
};
