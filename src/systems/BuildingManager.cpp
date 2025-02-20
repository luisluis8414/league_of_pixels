#include "BuildingManager.h"

#include "../components/buildings/Tower.h"
#include "../core/Config.h"
#include "../core/Utils.h"

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
  for (const std::shared_ptr<Tower>& tower : m_blueSideTowers) {
    std::shared_ptr<Minion> closestMinion = nullptr;
    float closestDistance = std::numeric_limits<float>::max();

    for (const std::shared_ptr<Minion>& minion : m_redSideMinions) {
      float distance = Utils::getVectorDistance(tower->getPosition(), minion->getPosition());
      if (Utils::isRectInCircle(minion->getHitbox(), tower->getRange()) && distance < closestDistance) {
        closestMinion = minion;
        closestDistance = distance;
      }
    }

    if (closestMinion) {
      tower->attackEntity(closestMinion);
    }
  }

  for (const std::shared_ptr<Tower>& tower : m_redSideTowers) {
    std::shared_ptr<Minion> closestMinion = nullptr;
    float closestDistance = std::numeric_limits<float>::max();

    for (const std::shared_ptr<Minion>& minion : m_blueSideMinions) {
      float distance = Utils::getVectorDistance(tower->getPosition(), minion->getPosition());
      if (Utils::isRectInCircle(minion->getHitbox(), tower->getRange()) && distance < closestDistance) {
        closestMinion = minion;
        closestDistance = distance;
      }
    }

    if (closestMinion) {
      tower->attackEntity(closestMinion);
    } else if (m_player->isAlive() && Utils::isRectInCircle(m_player->getHitbox(), tower->getRange())) {
      tower->attackEntity(m_player);
    }
  }
}

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