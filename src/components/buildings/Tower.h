#pragma once

#include <SFML/System/Clock.hpp>

#include "../../core/Event.h"
#include "../../entities/Archer.h"
#include "Building.h"

class Tower : public Building {
 public:
  Tower(EventDispatcher& dispatcher,
        const std::string& towerTexturePath,
        const std::string& archerTexturePath,
        sf::Vector2f position,
        float health = 100,
        float scale = 1.0f);

  const sf::CircleShape& getRange();

  void attackEntity(std::shared_ptr<Entity> entity);

 private:
  sf::CircleShape m_range;
  Archer m_archer;

  void onDraw(DrawEvent& event);
};