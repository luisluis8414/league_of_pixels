#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "../core/Event.h"
#include "../entities/Entity.h"

class Projectile {
 public:
  Projectile(EventDispatcher& dispatcher,
             const std::string& texturePath,
             sf::Vector2f position,
             std::shared_ptr<Entity> target,
             float velocity);

  ~Projectile();
  void update(float deltaTime);

 private:
  EventDispatcher& m_eventDispatcher;

  sf::Texture m_texture;
  sf::Sprite m_sprite;

  sf::Vector2f m_position;
  std::weak_ptr<Entity> m_target;
  sf::Vector2f m_direction;
  float m_velocity;
  float m_initialVelocity;
  sf::Clock m_lifetime;

  void onDraw(sf::RenderWindow& window);
};
