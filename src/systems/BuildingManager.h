#include "../core/Event.h"

class BuildingManager {
 public:
  BuildingManager(EventDispatcher& dispatcher);

  ~BuildingManager() = default;

 private:
  EventDispatcher& m_eventDispatcher;

  std::vector<std::shared_ptr<Tower>> m_blueSideTowers;
  std::vector<std::shared_ptr<Tower>> m_redSideTowers;

  void initBuildings();
};