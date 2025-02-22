#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "../core/Event.h"
#include "Entity.h"

enum class MinionAnimationState { WALKING, HITTING, DYING };

class Minion : public Entity {
 public:
  Minion(EventDispatcher& dispatcher,
         const std::string& texturePath,
         sf::Vector2f position,
         sf::Vector2f destination,
         sf::Color healthbarColor);

  ~Minion() = default;

  bool isHitting() const;

 private:
  MinionAnimationState m_state;

  sf::Color m_healthBarColor;

  void updateHealthBar() override;
  void updateHitbox() override;

  void onAnimationEnd() override;
  void setWalking() override;

  void setIdle() override;

  void onUpdate(float deltaTime) override;
  void onDraw(DrawEvent& event) override;

  void setAnimation(MinionAnimationState state);

  const std::unordered_map<MinionAnimationState, AnimationConfig> m_animationConfigs = {
      {MinionAnimationState::WALKING, {0, 5, 0.1f}},
      {MinionAnimationState::HITTING, {10, 19, 0.1f, 14}},
      {MinionAnimationState::DYING, {20, 26, 0.1f}},
  };

  virtual void onTargetInRange() override;
};
