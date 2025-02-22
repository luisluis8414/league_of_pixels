#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>  // Include for std::weak_ptr and std::shared_ptr
#include <optional>
#include <vector>

#include "../components/Map.h"
#include "../core/Event.h"
#include "../core/Utils.h"  // Make sure to include Utils here

enum class EntityType { Player, Enemy, Minion, Archer, Building };

struct AnimationConfig {
  int startFrame;
  int endFrame;
  float frameTime;
  int dmgFrame;
};

struct HitboxConfig {
  float widthFactor;
  float heightFactor;
  float offsetXFactor;
  float offsetYFactor;
};

class Entity {
 public:
  Entity(EventDispatcher& dispatcher,
         int frameWidth,
         int frameHeight,
         sf::Vector2f position,
         float frameTime,
         float speed,
         float maxHealth,
         float physicalDmg,
         EntityType type,
         std::string texturePath,
         RenderLayer renderLayer);

  ~Entity();

  EntityType getType() const;

  const sf::FloatRect& getHitbox() const;

  const sf::CircleShape& getRange();

  const sf::Vector2f getCenter();

  const sf::Vector2f getPosition();

  void takeDmg(float dmg);

  void setTarget(std::shared_ptr<Entity> target);
  void clearTarget();
  std::shared_ptr<Entity> getTarget() const;
  bool hasTarget() const;

  void setDestination(sf::Vector2f position);

 protected:
  EventDispatcher& m_eventDispatcher;

  sf::FloatRect m_hitbox;

  sf::Texture m_texture;
  sf::Sprite m_sprite;
  sf::IntRect m_frameRect;

  int m_startFrame;
  int m_endFrame;
  int m_currentFrame;
  float m_frameTime;
  float m_elapsedTime;
  int m_dmgFrame = 0;

  int m_frameWidth;
  int m_frameHeight;

  std::weak_ptr<Entity> m_target;  // Changed to weak_ptr
  sf::Vector2f m_destination;
  float m_speed;

  float m_maxHealth;
  float m_currentHealth;
  sf::CircleShape m_range;

  float m_physicalDmg;

  sf::RectangleShape m_healthBarBackground;
  sf::RectangleShape m_healthBarForeground;

  virtual void updateHealthBar() = 0;
  virtual void updateHitbox() = 0;

  void move(float deltaTime);

  virtual void setWalking() = 0;
  virtual void setIdle() = 0;

  virtual void setPosition(sf::Vector2f position);

  virtual void onUpdate(float deltaTime) = 0;
  virtual void onDraw(DrawEvent& event) = 0;

  virtual void onDmgFrame();

  EntityType m_type;

  virtual void onAnimationEnd() = 0;
  void updateAnimation(float deltaTime);

  // New methods
  virtual void onTargetInRange() {};  // Virtual function for derived classes to implement
  bool isTargetInRange(std::shared_ptr<Entity> target);
};
