#include "Building.h"

#include <iostream>

#include "../../core/Config.h"

Building::Building(
    EventDispatcher& dispatcher, const std::string& texturePath, sf::Vector2f position, float health, float scale)
    : Entity(dispatcher, 0, 0, position, 0, 0, 1000.f, 0.f, EntityType::Building, texturePath), m_position(position) {
  if (!m_texture.loadFromFile(texturePath)) {
    std::cerr << "Failed to load texture: " << texturePath << std::endl;
  }
  m_sprite.setTextureRect(sf::IntRect({0, 0}, {(int)m_texture.getSize().x, (int)m_texture.getSize().y}));

  m_texture.setSmooth(false);
  m_sprite.setTexture(m_texture);
  m_sprite.setPosition(position);
  m_sprite.setScale({scale, scale});

  m_healthBarBackground.setSize(sf::Vector2f(100.f, 10.f));
  m_healthBarBackground.setFillColor(sf::Color::Black);

  m_healthBarForeground.setSize(sf::Vector2f(100.f, 10.f));
  m_healthBarForeground.setFillColor(sf::Color::Green);

  sf::FloatRect spriteBounds = m_sprite.getGlobalBounds();

  float hitboxWidth = spriteBounds.size.x * 0.5f;
  float hitboxHeight = spriteBounds.size.y * 0.6f;

  float hitboxLeft = spriteBounds.position.x + (spriteBounds.size.x - hitboxWidth) / 2.f;
  float hitboxTop = spriteBounds.position.y + (spriteBounds.size.y - hitboxHeight) / 2.f + 20;

  m_hitbox = sf::FloatRect({hitboxLeft, hitboxTop}, {hitboxWidth, hitboxHeight});

  m_eventDispatcher.subscribe<DrawEvent>(
      this, [this](DrawEvent& event) { this->onDraw(event); }, RenderLayer::BUILDINGS);

  m_eventDispatcher.subscribe<TickEvent>(this, [this](TickEvent& event) { this->onUpdate(event.getDeltaTime()); });
}

void Building::onUpdate(float deltaTime) {
  updateHealthBar();
}

void Building::onDraw(DrawEvent& event) {
  sf::RenderWindow& window = event.getWindow();
  window.draw(m_sprite);

  window.draw(m_healthBarBackground);
  window.draw(m_healthBarForeground);

  if (Config::Settings::showHitboxes) {
    sf::RectangleShape hitboxShape;
    hitboxShape.setPosition({m_hitbox.position.x, m_hitbox.position.y});
    hitboxShape.setSize(sf::Vector2f(m_hitbox.size.x, m_hitbox.size.y));
    hitboxShape.setFillColor(sf::Color::Transparent);
    hitboxShape.setOutlineColor(sf::Color::Red);
    hitboxShape.setOutlineThickness(1.f);

    window.draw(hitboxShape);
  }
}

sf::Vector2f Building::getCenterPosition() {
  sf::FloatRect bounds = m_sprite.getGlobalBounds();

  sf::Vector2f center;
  center.x = bounds.position.x + bounds.size.x / 2.0f;
  center.y = bounds.position.y + bounds.size.y / 2.0f;

  return center;
}

void Building::updateHealthBar() {
  if (m_currentHealth <= 0) {
    DestroyEntityEvent destroyEvent(this);
    m_eventDispatcher.emit(destroyEvent);
    return;
  }

  float healthPercentage = m_currentHealth / m_maxHealth;
  m_healthBarForeground.setSize(sf::Vector2f(healthPercentage * 100.f, 10.f));

  sf::FloatRect bounds = m_sprite.getGlobalBounds();
  float healthBarX = bounds.position.x + (bounds.size.x / 2.f) - (m_healthBarBackground.getSize().x / 2.f);
  float healthBarY = bounds.position.y - m_healthBarBackground.getSize().y + 25.f;  // offset for spacing from top
  m_healthBarBackground.setPosition({healthBarX, healthBarY});
  m_healthBarForeground.setPosition({healthBarX, healthBarY});
}

void Building::updateHitbox() {};
void Building::setIdle() {};
void Building::setWalking() {};
void Building::onAnimationEnd() {};
