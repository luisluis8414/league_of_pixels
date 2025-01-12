#include "../core/Event.h"
#include "Entity.h"

enum class ArcherAnimationState { IDLE, SHOOTING };

class Archer : public Entity {
 public:
  Archer(EventDispatcher& dispatcher, sf::Vector2f position, const std::string& texturePath);

  void attackEntity(const Entity& entity);

 private:
  ArcherAnimationState m_state;

  const std::unordered_map<ArcherAnimationState, AnimationConfig> m_animationConfigs = {
      {ArcherAnimationState::IDLE, {0, 5, 0.1f}},
      {ArcherAnimationState::SHOOTING, {25, 33, 0.1f}},
  };

  void onUpdate(float deltaTime) override;
  void onDraw(DrawEvent& event) override;

  void onAnimationEnd() override;

  void setAnimation(ArcherAnimationState state);

  void move(float deltaTime) {};

  void updateHealthBar() {};
  void updateHitbox() {};
};