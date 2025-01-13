#include "../components/buildings/Building.h"
#include "../core/Event.h"

class BuildingManager {
 public:
  BuildingManager(EventDispatcher& dispatcher,
                  std::vector<std::shared_ptr<Tower>>& m_blueSideTowers,
                  std::vector<std::shared_ptr<Tower>>& m_redSideTowers);

  ~BuildingManager() = default;

 private:
  EventDispatcher& m_eventDispatcher;

  Building m_blueSideNexus;
  Building m_redSideNexus;

  std::vector<std::shared_ptr<Tower>>& m_blueSideTowers;
  std::vector<std::shared_ptr<Tower>>& m_redSideTowers;

  void initBuildings();
};