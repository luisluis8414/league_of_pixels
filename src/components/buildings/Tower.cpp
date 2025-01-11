#include "Tower.h"

#include <iostream>

Tower::Tower(EventDispatcher& dispatcher,
             const std::string& towerTexturePath,
             const std::string& archerTexturePath,
             sf::Vector2f position,
             float health,
             float scale)
    : Building(dispatcher, towerTexturePath, position, health, scale),
      m_aaRange(20.f),
      m_archerSprite(m_archerTexture) {
  if (!m_archerTexture.loadFromFile(archerTexturePath)) {
    std::cerr << "Failed to load texture: " << archerTexturePath << std::endl;
  }
  m_archerSprite.setTextureRect(sf::IntRect({0, 0}, {192, 192}));

  m_archerTexture.setSmooth(false);
  m_archerSprite.setTexture(m_archerTexture);
  m_archerSprite.setPosition({position.x - 30.f, position.y - 20.f});

  dispatcher.subscribe<DrawEvent>(this, [this](DrawEvent& event) { onDraw(event); }, RenderLayer::ENTITIES);
}

void Tower::onDraw(DrawEvent& event) {
  sf::RenderWindow& window = event.getWindow();
  window.draw(m_archerSprite);
}