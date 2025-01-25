#pragma once

#include "../../core/Event.h"
#include "../../entities/Entity.h"
#include "SFML/Graphics.hpp"

class Building : public Entity {
 public:
  Building(EventDispatcher& dispatcher,
           const std::string& texturePath,
           sf::Vector2f position,
           float health = 100,
           float scale = 1.0f);

  sf::Vector2f getCenterPosition();

 protected:
  void onDraw(DrawEvent& event) override;

 private:
  sf::Vector2f m_position;

  void updateHealthBar();

  void updateHitbox() override;
  void setIdle() override;
  void setWalking() override;
  void onUpdate(float deltaTime) override;
  void onAnimationEnd() override;
};
