#pragma once

#include "../components/buildings/Tower.h"
#include "../entities/Minion.h"

class MinionManager {
 public:
  MinionManager(EventDispatcher& dispatcher,
                std::vector<std::shared_ptr<Minion>>& blueSideMinions,
                std::vector<std::shared_ptr<Minion>>& redSideMinions,
                std::vector<std::shared_ptr<Tower>>& blueSideTowers,
                std::vector<std::shared_ptr<Tower>>& redSideTowers);

  ~MinionManager() = default;

 private:
  EventDispatcher& m_eventDispatcher;

  std::vector<std::shared_ptr<Minion>>& m_blueSideMinions;
  std::vector<std::shared_ptr<Minion>>& m_redSideMinions;

  std::vector<std::shared_ptr<Tower>>& m_blueSideTowers;
  std::vector<std::shared_ptr<Tower>>& m_redSideTowers;

  int m_secondsSinceLastSpawnIt = 0;
  sf::Clock m_spawnClock;
  int m_minionsSpawned = 0;
  bool m_spawnCycleActive = false;

  void spawnMinions();
  void checkForTargets();
};