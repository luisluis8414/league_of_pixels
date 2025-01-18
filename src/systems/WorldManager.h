#pragma once

#include <memory>
#include <vector>

#include "../components/buildings/Tower.h"
#include "../core/Event.h"
#include "../entities/Enemy.h"
#include "../entities/Entity.h"
#include "../entities/Minion.h"
#include "../entities/Player.h"
#include "BuildingManager.h"
#include "MinionManager.h"

class WorldManager {
 public:
  WorldManager(EventDispatcher& dispatcher,
               std::shared_ptr<Player> player,
               std::vector<std::shared_ptr<Enemy>>& enemies,
               std::vector<std::shared_ptr<Minion>>& blueSideMinions,
               std::vector<std::shared_ptr<Minion>>& redSideMinions,
               std::vector<std::shared_ptr<Tower>>& blueSideTowers,
               std::vector<std::shared_ptr<Tower>>& redSideTowers);

  ~WorldManager() = default;

  void checkCollisions();

 private:
  EventDispatcher& m_eventDispatcher;
  BuildingManager m_buildingManager;

  std::shared_ptr<Player> m_player;
  std::vector<std::shared_ptr<Enemy>>& m_enemies;
  std::vector<std::shared_ptr<Minion>>& m_blueSideMinions;
  std::vector<std::shared_ptr<Minion>>& m_redSideMinions;
  std::vector<std::shared_ptr<Tower>>& m_blueSideTowers;

  MinionManager m_minionsManager;

  std::vector<Entity*> m_entitiesToDestroy;

  void checkForTarget(sf::Vector2f position);
  void checkAbilityDmg(sf::FloatRect hitbox, float spellDmg);

  void spawnEnemy(const std::string& texturePath, sf::Vector2f position);
  void init();

  void cleanUp();
};
