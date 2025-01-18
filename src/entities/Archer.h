#pragma once

#include "../core/Event.h"
#include "Entity.h"

enum class ArcherAnimationState { IDLE, SHOOTING };

class Archer : public Entity {
 public:
  Archer(EventDispatcher& dispatcher,
         sf::Vector2f position,
         const std::string& texturePath,
         std::weak_ptr<Entity>& target);

  void attackEntity(std::weak_ptr<Entity>& target);

 private:
  ArcherAnimationState m_state;
  std::weak_ptr<Entity>& m_target;

  const std::unordered_map<ArcherAnimationState, AnimationConfig> m_animationConfigs = {
      {ArcherAnimationState::IDLE, {0, 5, 0.1f}},
      {ArcherAnimationState::SHOOTING, {41, 48, 0.1f, 46}},
  };

  void onUpdate(float deltaTime) override;
  void onDraw(DrawEvent& event) override;

  void onAnimationEnd() override;

  void setAnimation(ArcherAnimationState state);

  void move(float deltaTime) {};

  void updateHealthBar() {};
  void updateHitbox() {};

  void setWalking() override;
  void setIdle() override;

  void onDmgFrame() override;
};