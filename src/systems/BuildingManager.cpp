#include "BuildingManager.h"

#include "../components/buildings/Tower.h"
#include "../core/Config.h"

BuildingManager::BuildingManager(EventDispatcher& dispatcher) : m_eventDispatcher(dispatcher) {
  m_eventDispatcher.subscribe<InitEvent>(this, [this](InitEvent) { this->initBuildings(); });
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