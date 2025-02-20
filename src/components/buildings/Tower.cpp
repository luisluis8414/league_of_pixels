#include "Tower.h"

#include <iostream>

#include "../../core/Config.h"
#include "../../core/Utils.h"
#include "../../projectiles/projectile.h"

Tower::Tower(EventDispatcher& dispatcher,
             const std::string& towerTexturePath,
             const std::string& archerTexturePath,
             sf::Vector2f position,
             float health,
             float scale)
    : Building(dispatcher, towerTexturePath, position, health, scale),
      m_archer(dispatcher, {position.x + 65, position.y + 75}, archerTexturePath, m_target) {
  m_range.setRadius(250.f);
  m_range.setOrigin({m_range.getRadius(), m_range.getRadius()});
  m_range.setPosition(getCenter());
  m_range.setFillColor(sf::Color(0, 0, 0, 0));
  m_range.setOutlineColor(sf::Color::Red);
  m_range.setOutlineThickness(1.5f);

  dispatcher.subscribe<DrawEvent>(this, [this](DrawEvent& event) { this->onDraw(event); });
}

const sf::Vector2f Tower::getPosition() {
  return m_archer.getPosition();
}

void Tower::attackEntity(std::shared_ptr<Entity> entity) {
  if (!hasTarget()) {
    m_target = entity;
  }
  m_archer.attackEntity(m_target);
}

bool Tower::hasTarget() {
  if (std::shared_ptr<Entity> target = m_target.lock()) {
    return !m_target.expired() && Utils::isRectInCircle(target->getHitbox(), getRange());
  } else {
    return false;
  }
}

void Tower::onDraw(DrawEvent& event) {
  sf::RenderWindow& window = event.getWindow();
  Building::onDraw(event);

  if (Config::Settings::showHitboxes) {
    window.draw(m_range);
  }
};