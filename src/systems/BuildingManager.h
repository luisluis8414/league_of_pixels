#pragma once

#include "../components/buildings/Building.h"
#include "../components/buildings/Tower.h"
#include "../core/Event.h"
#include "../entities/Minion.h"
#include "../entities/Player.h"

class BuildingManager {
 public:
  BuildingManager(EventDispatcher& dispatcher,
                  std::shared_ptr<Player> player,
                  std::vector<std::shared_ptr<Tower>>& blueSideTowers,
                  std::vector<std::shared_ptr<Tower>>& redSideTowers,
                  std::vector<std::shared_ptr<Minion>>& blueSideMinions,
                  std::vector<std::shared_ptr<Minion>>& redSideMinions);

  ~BuildingManager() = default;

 private:
  EventDispatcher& m_eventDispatcher;

  std::shared_ptr<Player> m_player;

  Building m_blueSideNexus;
  Building m_redSideNexus;

  std::vector<std::shared_ptr<Tower>>& m_blueSideTowers;
  std::vector<std::shared_ptr<Tower>>& m_redSideTowers;

  std::vector<std::shared_ptr<Minion>>& m_blueSideMinions;
  std::vector<std::shared_ptr<Minion>>& m_redSideMinions;

  void initBuildings();

  void checkForTargets();
};