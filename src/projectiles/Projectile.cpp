#include "projectile.h"

#include <iostream>

Projectile::Projectile(EventDispatcher& dispatcher,
                       const std::string& texturePath,
                       sf::Vector2f position,
                       std::shared_ptr<Entity> target,
                       float velocity)
    : m_eventDispatcher(dispatcher),
      m_position(position),
      m_sprite(m_texture),
      m_target(target),
      m_velocity(velocity),
      m_initialVelocity(velocity) {
  if (!m_texture.loadFromFile(texturePath)) {
    std::cerr << "Failed to load sprite sheet: " << texturePath << std::endl;
  }

  m_texture.setSmooth(false);
  m_sprite.setTexture(m_texture);

  m_sprite.setTextureRect(sf::IntRect({0, 0}, {(int)m_texture.getSize().x, (int)m_texture.getSize().y / 2}));

  m_sprite.setOrigin({m_texture.getSize().x / 2.f, m_texture.getSize().y / 4.f});

  m_sprite.setPosition(position);

  sf::Vector2f direction = m_target->getPosition() - m_position;
  float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
  m_direction = (magnitude > 0.0f) ? direction / magnitude : sf::Vector2f(0.f, 0.f);

  m_eventDispatcher.subscribe<DrawEvent>(
      this, [this](DrawEvent& event) { this->onDraw(event.getWindow()); }, RenderLayer::ENTITIES);
};

void Projectile::onDraw(sf::RenderWindow& window) {
  window.draw(m_sprite);
};

Projectile::~Projectile() {
  m_eventDispatcher.unsubscribe(this);
}

void Projectile::update(float deltaTime) {
  float elapsedTime = m_lifetime.getElapsedTime().asSeconds();
  m_velocity = m_initialVelocity + elapsedTime * 1000.f;

  sf::Vector2f direction = m_target->getPosition() - m_position;
  float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
  m_direction = (magnitude > 0.0f) ? direction / magnitude : sf::Vector2f(0.f, 0.f);

  m_position += m_direction * m_velocity * deltaTime;
  m_sprite.setPosition(m_position);

  float angle = std::atan2(m_direction.y, m_direction.x) * 180.f / 3.14159265f;
  m_sprite.setRotation(sf::degrees(angle));

  if (std::hypot(m_position.x - m_target->getPosition().x, m_position.y - m_target->getPosition().y) <=
      m_velocity * deltaTime) {
    m_target->takeDmg(10.f);
    DestroyProjectileEvent event(this);
    m_eventDispatcher.emit(event);
  }
}
