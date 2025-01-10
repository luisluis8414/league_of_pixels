#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include <vector>

#include "../Event.h"

enum class EntityType { Player, Enemy, Minion };

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

// inline uint64_t generateRandomId() {
//     static std::random_device rd;
//     static std::mt19937_64 gen(rd());
//     static std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);
//     return dist(gen);
// }

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
         std::string texturePath)
      : m_dispatcher(dispatcher),
        m_frameWidth(frameWidth),
        m_frameHeight(frameHeight),
        m_elapsedTime(0.f),
        m_startFrame(0),
        m_endFrame(5),
        m_currentFrame(0),
        m_frameTime(frameTime),
        m_speed(speed),
        m_maxHealth(maxHealth),
        m_currentHealth(maxHealth),
        m_physicalDmg(physicalDmg),
        m_type(type),
        m_destination(position),
        m_target(std::nullopt),
        m_sprite(m_texture) {
    if (!m_texture.loadFromFile(texturePath)) {
      std::cerr << "Failed to load sprite sheet: " << texturePath << std::endl;
    }

    m_texture.setSmooth(false);
    m_sprite.setTexture(m_texture);

    m_frameRect = sf::IntRect({0, 0}, {m_frameWidth, m_frameHeight});
    m_sprite.setTextureRect(m_frameRect);

    m_sprite.setPosition({position.x, position.y});

    m_sprite.setOrigin({m_sprite.getGlobalBounds().size.x / 2.f, m_sprite.getGlobalBounds().size.y / 2.f});

    m_healthBarBackground.setSize(sf::Vector2f(100.f, 10.f));
    m_healthBarBackground.setFillColor(sf::Color::Red);
    m_healthBarBackground.setPosition({10.f, 10.f});

    m_healthBarForeground.setSize(sf::Vector2f(100.f, 10.f));
    m_healthBarForeground.setFillColor(sf::Color::Green);
    m_healthBarForeground.setPosition({10.f, 10.f});
  }

  ~Entity() {
    m_dispatcher.unsubscribe(this);
  }

  EntityType getType() const {
    return m_type;
  }
  const sf::FloatRect& getHitbox() const {
    return m_hitbox;
  }

  const sf::Vector2f getSpriteCenter() {
    sf::Vector2f currentPosition = m_sprite.getPosition();

    sf::FloatRect bounds = m_sprite.getGlobalBounds();

    return sf::Vector2f(currentPosition.x + bounds.size.x / 2.f, currentPosition.y + bounds.size.y / 2.f);
  }

  sf::Vector2f getPosition() {
    return m_sprite.getPosition();
  }

  void takeDmg(float dmg) {
    m_currentHealth -= dmg;
  }

 protected:
  EventDispatcher& m_dispatcher;

  sf::FloatRect m_hitbox;

  sf::Texture m_texture;
  sf::Sprite m_sprite;
  sf::IntRect m_frameRect;

  int m_startFrame;
  int m_endFrame;
  int m_currentFrame;
  float m_frameTime;
  float m_elapsedTime;

  int m_frameWidth;
  int m_frameHeight;

  std::optional<Entity*> m_target;
  sf::Vector2f m_destination;
  float m_speed;

  float m_maxHealth;
  float m_currentHealth;

  float m_physicalDmg;

  sf::RectangleShape m_healthBarBackground;
  sf::RectangleShape m_healthBarForeground;

  virtual void updateHealthBar() = 0;
  virtual void updateHitbox() = 0;

  virtual void move(float deltaTime) = 0;
  virtual void updateAnimation(float deltaTime) = 0;

  virtual void setPosition(sf::Vector2f position) {
    m_sprite.setPosition({position.x, position.y});
  };

  virtual void onUpdate(float deltaTime) = 0;
  virtual void onDraw(DrawEvent& event) = 0;

  virtual void onCollision() = 0;

  void setDestination(sf::Vector2f position) {
    sf::Vector2f direction = position - m_sprite.getPosition();

    constexpr float flipThreshold = 1.f;

    if (direction.x < -flipThreshold && m_sprite.getScale().x != -1.f) {
      m_sprite.setScale({-1.f, 1.f});
    } else if (direction.x > flipThreshold && m_sprite.getScale().x != 1.f) {
      m_sprite.setScale({1.f, 1.f});
    }

    m_destination = position;
  }

  EntityType m_type;
};
