#include "../entities/Minion.h"

class MinionManager {
 public:
  MinionManager(EventDispatcher& dispatcher,
                std::vector<std::shared_ptr<Minion>>& blueSideMinions,
                std::vector<std::shared_ptr<Minion>>& redSideMinions);

  ~MinionManager() = default;

 private:
  EventDispatcher& m_eventDispatcher;

  std::vector<std::shared_ptr<Minion>>& m_blueSideMinions;
  std::vector<std::shared_ptr<Minion>>& m_redSideMinions;

  int m_secondsSinceLastSpawnIt = 0;
  sf::Clock m_spawnClock;
  int m_minionsSpawned = 0;
  bool m_spawnCycleActive = false;

  void spawnMinions();
};