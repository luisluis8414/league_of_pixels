#pragma once

#include "../../core/Event.h"
#include "SFML/Graphics.hpp"

class Building {
 public:
  Building(EventDispatcher& dispatcher,
           const std::string& texturePath,
           sf::Vector2f position,
           float health = 100,
           float scale = 1.0f);

  void onDraw(DrawEvent& event);

  sf::Vector2f getCenterPosition();

 protected:
  EventDispatcher& m_eventDispatcher;

 private:
  sf::Texture m_texture;
  sf::Sprite m_sprite;
  sf::Vector2f m_position;

  float m_health;
};
