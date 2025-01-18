#include "MinionManager.h"

#include "../core/Config.h"

MinionManager::MinionManager(EventDispatcher& dispatcher,
                             std::vector<std::shared_ptr<Minion>>& blueSideMinions,
                             std::vector<std::shared_ptr<Minion>>& redSideMinions)
    : m_blueSideMinions(blueSideMinions), m_redSideMinions(redSideMinions), m_eventDispatcher(dispatcher) {
  m_eventDispatcher.subscribe<SecondsEvent>(this, [this](SecondsEvent& event) {
    m_secondsSinceLastSpawnIt++;
    if (m_secondsSinceLastSpawnIt >= Config::Minions::SPAWN_TIMER || m_spawnCycleActive) {
      spawnMinions();
      m_secondsSinceLastSpawnIt = 0;
    }
  });
};

void MinionManager::spawnMinions() {
  if (!m_spawnCycleActive) {
    m_spawnCycleActive = true;
    m_spawnClock.restart();
    m_minionsSpawned = 0;
  }

  if (m_spawnCycleActive) {
    float elapsed = m_spawnClock.getElapsedTime().asSeconds();

    if (elapsed >= 1.f && m_minionsSpawned < 3) {
      m_blueSideMinions.push_back(std::make_shared<Minion>(m_eventDispatcher,
                                                           Config::Textures::Troops::MINIONS_BLUE,
                                                           Config::Minions::BLUE_SIDE_SPAWN,
                                                           Config::Minions::RED_SIDE_SPAWN,
                                                           sf::Color::Blue));
      m_redSideMinions.push_back(std::make_shared<Minion>(m_eventDispatcher,
                                                          Config::Textures::Troops::MINIONS_RED,
                                                          Config::Minions::RED_SIDE_SPAWN,
                                                          Config::Minions::BLUE_SIDE_SPAWN,
                                                          sf::Color::Red));

      m_spawnClock.restart();
      m_minionsSpawned++;
    }

    if (m_minionsSpawned >= 3) {
      m_spawnCycleActive = false;
    }
  }
}