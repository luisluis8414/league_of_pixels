#include "Building.h"

#include <iostream>

Building::Building(
    EventDispatcher& dispatcher, const std::string& texturePath, sf::Vector2f position, float health, float scale)
    : m_eventDispatcher(dispatcher), m_position(position), m_sprite(m_texture), m_health(health) {
  if (!m_texture.loadFromFile(texturePath)) {
    std::cerr << "Failed to load texture: " << texturePath << std::endl;
  }
  m_sprite.setTextureRect(sf::IntRect({0, 0}, {(int)m_texture.getSize().x, (int)m_texture.getSize().y}));

  m_texture.setSmooth(false);
  m_sprite.setTexture(m_texture);
  m_sprite.setPosition(position);
  m_sprite.setScale({scale, scale});

  dispatcher.subscribe<DrawEvent>(this, [this](DrawEvent& event) { this->onDraw(event); }, RenderLayer::BUILDINGS);
}

void Building::onDraw(DrawEvent& event) {
  sf::RenderWindow& window = event.getWindow();
  window.draw(m_sprite);
}

sf::Vector2f Building::getCenterPosition() {
  sf::FloatRect bounds = m_sprite.getGlobalBounds();

  sf::Vector2f center;
  center.x = bounds.position.x + bounds.size.x / 2.0f;
  center.y = bounds.position.y + bounds.size.y / 2.0f;

  return center;
}
