#include "Tower.h"

#include <iostream>

#include "../../core/Config.h"
#include "../../projectiles/projectile.h"

Tower::Tower(EventDispatcher& dispatcher,
             const std::string& towerTexturePath,
             const std::string& archerTexturePath,
             sf::Vector2f position,
             float health,
             float scale)
    : Building(dispatcher, towerTexturePath, position, health, scale),
      m_archer(dispatcher, {position.x + 65, position.y + 75}, archerTexturePath) {
  m_range.setRadius(200.f);
  m_range.setOrigin({m_range.getRadius(), m_range.getRadius()});
  m_range.setPosition(getCenterPosition());
  m_range.setFillColor(sf::Color(0, 0, 0, 0));
  m_range.setOutlineColor(sf::Color::Red);
  m_range.setOutlineThickness(1.5f);

  dispatcher.subscribe<DrawEvent>(this, [this](DrawEvent& event) { this->onDraw(event); });
}

const sf::CircleShape& Tower::getRange() {
  return m_range;
};

void Tower::attackEntity(std::shared_ptr<Entity> entity) {
  m_archer.attackEntity(entity);
}

void Tower::onDraw(DrawEvent& event) {
  sf::RenderWindow& window = event.getWindow();

  if (Config::Settings::showHitboxes) {
    window.draw(m_range);
  }
};