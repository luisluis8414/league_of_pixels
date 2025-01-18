#include "BuildingManager.h"

#include "../components/buildings/Tower.h"
#include "../core/Config.h"

BuildingManager::BuildingManager(EventDispatcher& dispatcher,
                                 std::shared_ptr<Player> player,
                                 std::vector<std::shared_ptr<Tower>>& blueSideTowers,
                                 std::vector<std::shared_ptr<Tower>>& redSideTowers,
                                 std::vector<std::shared_ptr<Minion>>& blueSideMinions,
                                 std::vector<std::shared_ptr<Minion>>& redSideMinions)
    : m_eventDispatcher(dispatcher),
      m_player(player),
      m_blueSideTowers(blueSideTowers),
      m_redSideTowers(redSideTowers),
      m_blueSideNexus(m_eventDispatcher, Config::Textures::Buildings::BLUE_SIDE_NEXUS, {300.f, 400.f}),
      m_redSideNexus(m_eventDispatcher, Config::Textures::Buildings::RED_SIDE_NEXUS, {3500.f, 400.f}),
      m_blueSideMinions(blueSideMinions),
      m_redSideMinions(redSideMinions) {
  m_eventDispatcher.subscribe<InitEvent>(this, [this](InitEvent) { this->initBuildings(); });

  m_eventDispatcher.subscribe<TickEvent>(this, [this](TickEvent) { this->checkForTargets(); });
}

void BuildingManager::checkForTargets() {
  for (const std::shared_ptr<Tower> tower : m_blueSideTowers) {
    for (std::shared_ptr<Minion> minion : m_redSideMinions) {
      if (this->isHitboxInRange(minion->getHitbox(), tower->getRange())) {
        tower->attackEntity(minion);
      }
    }
  }

  for (const std::shared_ptr<Tower> tower : m_redSideTowers) {
    for (std::shared_ptr<Minion> minion : m_blueSideMinions) {
      if (this->isHitboxInRange(minion->getHitbox(), tower->getRange())) {
        tower->attackEntity(minion);
      }
    }
    if (m_player->isAlive() && isHitboxInRange(m_player->getHitbox(), tower->getRange())) {
      tower->attackEntity(m_player);
    }
  }
}

bool BuildingManager::isHitboxInRange(const sf::FloatRect& hitbox, const sf::CircleShape& range) {
  sf::Vector2f circleCenter = range.getPosition();
  float radius = range.getRadius();

  sf::Vector2f rectCenter(hitbox.position.x + hitbox.size.x / 2.f, hitbox.position.y + hitbox.size.y / 2.f);

  float closestX = std::max(hitbox.position.x, std::min(circleCenter.x, hitbox.position.x + hitbox.size.x));
  float closestY = std::max(hitbox.position.y, std::min(circleCenter.y, hitbox.position.y + hitbox.size.y));

  float dx = circleCenter.x - closestX;
  float dy = circleCenter.y - closestY;

  if ((dx * dx + dy * dy) <= (radius * radius)) {
    return true;
  }
  return false;
};

void BuildingManager::initBuildings() {
  m_blueSideTowers.push_back(std::make_shared<Tower>(m_eventDispatcher,
                                                     Config::Textures::Buildings::T3_BLUE_SIDE_TOWER,
                                                     Config::Textures::Troops::BLUE_SIDE_ARCHER,
                                                     sf::Vector2f(1600.f, 400.f),
                                                     1000.f,
                                                     1.f));

  m_blueSideTowers.push_back(std::make_shared<Tower>(m_eventDispatcher,
                                                     Config::Textures::Buildings::T3_BLUE_SIDE_TOWER,
                                                     Config::Textures::Troops::BLUE_SIDE_ARCHER,
                                                     sf::Vector2f(1100.f, 400.f),
                                                     1000.f,
                                                     1.f));

  m_blueSideTowers.push_back(std::make_shared<Tower>(m_eventDispatcher,
                                                     Config::Textures::Buildings::T3_BLUE_SIDE_TOWER,
                                                     Config::Textures::Troops::BLUE_SIDE_ARCHER,
                                                     sf::Vector2f(600.f, 300.f),
                                                     1000.f,
                                                     1.f));

  m_blueSideTowers.push_back(std::make_shared<Tower>(m_eventDispatcher,
                                                     Config::Textures::Buildings::T3_BLUE_SIDE_TOWER,
                                                     Config::Textures::Troops::BLUE_SIDE_ARCHER,
                                                     sf::Vector2f(600.f, 500.f),
                                                     1000.f,
                                                     1.f));

  m_redSideTowers.push_back(std::make_shared<Tower>(m_eventDispatcher,
                                                    Config::Textures::Buildings::T3_RED_SIDE_TOWER,
                                                    Config::Textures::Troops::RED_SIDE_ARCHER,
                                                    sf::Vector2f{2400.f, 400.f},
                                                    1000.f,
                                                    1.f));

  m_redSideTowers.push_back(std::make_shared<Tower>(m_eventDispatcher,
                                                    Config::Textures::Buildings::T3_RED_SIDE_TOWER,
                                                    Config::Textures::Troops::RED_SIDE_ARCHER,
                                                    sf::Vector2f{2900.f, 400.f},
                                                    1000.f,
                                                    1.f));

  m_redSideTowers.push_back(std::make_shared<Tower>(m_eventDispatcher,
                                                    Config::Textures::Buildings::T3_RED_SIDE_TOWER,
                                                    Config::Textures::Troops::RED_SIDE_ARCHER,
                                                    sf::Vector2f{3400.f, 300.f},
                                                    1000.f,
                                                    1.f));

  m_redSideTowers.push_back(std::make_shared<Tower>(m_eventDispatcher,
                                                    Config::Textures::Buildings::T3_RED_SIDE_TOWER,
                                                    Config::Textures::Troops::RED_SIDE_ARCHER,
                                                    sf::Vector2f{3400.f, 500.f},
                                                    1000.f,
                                                    1.f));
}