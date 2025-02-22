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

  const sf::FloatRect& getAttackHitbox() const {
    return m_attackHitbox;
  };

  bool isHitting() const;
  // void attackEntity(std::shared_ptr<Entity> entity); // Removed
  // bool hasTarget(); // Removed

 private:
  MinionAnimationState m_state;
  sf::FloatRect m_attackHitbox;

  sf::Color m_healthBarColor;

  void updateHealthBar() override;
  void updateHitbox() override;

  void onAnimationEnd() override;

  void setWalking() override;
  void setIdle() override;

  void onUpdate(float deltaTime) override;
  void onDraw(DrawEvent& event) override;

  void setAnimation(MinionAnimationState state);

  // std::weak_ptr<Entity> m_target; // Removed

  const std::unordered_map<MinionAnimationState, AnimationConfig> m_animationConfigs = {
      {MinionAnimationState::WALKING, {0, 5, 0.1f}},
      {MinionAnimationState::HITTING, {6, 15, 0.1f, 11}},  
      {MinionAnimationState::DYING, {16, 22, 0.1f}},
  };

  std::unordered_map<MinionAnimationState, HitboxConfig> m_attackHitboxConfigs = {
      //{float widthFactor; heightFactor; offsetXFactor; offsetYFactor; }
      {MinionAnimationState::HITTING, {0.35f, 0.5f, 0.6f, 0.2f}},
  };

  virtual void onTargetInRange() override;
  virtual void onDmgFrame() override;
};
