#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "../core/Event.h"
#include "Ability.h"
#include "Entity.h"

enum class PlayerAnimationState {
  IDLE,
  WALKING,
  AA1,
  AA2,
  // SlashForwardLeft,
  // SlashForwardRight,
  // SlashBehindLeft,
  // SlashBehindRight
};

enum class PlayerAbilities { Q, W, E, R };

struct AbilityAnimationState {
  bool isAbilityActive = false;
  sf::Vector2f abilityPosition;
  int abilityCurrentFrame = 0;
  float abilityElapsedTime = 0.f;
  float abilityFrameDuration = 0.1f;
  int abilityTotalFrames = 10;
};

class Player : public Entity {
 public:
  Player(EventDispatcher& dispatcher,
         sf::Vector2f position,
         std::string qTexturePath,
         std::string wTexturePath,
         std::string eTexturePath,
         std::string rTexturePath);

  ~Player() = default;

  const sf::FloatRect& getAttackHitbox() const {
    return m_attackHitbox;
  };

  bool isHitting() const;

 private:
  PlayerAnimationState m_state;
  sf::FloatRect m_attackHitbox;

  sf::Texture m_qTexture;
  sf::Texture m_wTexture;
  sf::Texture m_eTexture;
  sf::Texture m_rTexture;

  std::vector<Ability> m_activeAbilities;

  AbilityAnimationState m_qAnimationState;

  void updateHealthBar() override;
  void updateHitbox() override;

  void move(float deltaTime) override;
  // void updateAnimation(float deltaTime) override;
  void onAnimationEnd() override;

  void onUpdate(float deltaTime) override;
  void onDraw(DrawEvent& event) override;

  void setAnimation(PlayerAnimationState state);
  void castAbility(PlayerAbilities ability, sf::Vector2f position);

  void updateAbilities(float deltaTime);
  void drawAbilities(sf::RenderWindow& window);

  void checkTargetInRange(Entity* target);
  void autoAttack();

  const std::unordered_map<PlayerAnimationState, AnimationConfig> m_animationConfigs = {
      {PlayerAnimationState::IDLE, {0, 5, 0.1f}},
      {PlayerAnimationState::WALKING, {6, 11, 0.1f}},
      {PlayerAnimationState::AA1, {12, 17, 0.1f, 15}},
      {PlayerAnimationState::AA2, {18, 23, 0.1f, 21}},
      // {PlayerAnimationState::SlashForwardLeft, {24, 29, 0.1f}},
      // {PlayerAnimationState::SlashForwardRight, {30, 35, 0.1f}},
      // {PlayerAnimationState::SlashBehindLeft, {36, 41, 0.1f}},
      // {PlayerAnimationState::SlashBehindRight, {42, 47, 0.1f}},
  };

  // std::unordered_map<PlayerAnimationState, HitboxConfig> m_attackHitboxConfigs = {
  //     // { float widthFactor; heightFactor; offsetXFactor; offsetYFactor;
  //     // }
  //     {PlayerAnimationState::SlashForwardRight, {0.6f, 0.3f, 0.3f, 0.55f}},
  //     {PlayerAnimationState::SlashBehindLeft, {0.6f, 0.25f, 0.2f, 0.2f}},
  // };
};
