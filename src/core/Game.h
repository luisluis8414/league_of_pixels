#pragma once

#include "../components/Camera.h"
#include "../components/Map.h"
#include "../components/buildings/Building.h"
#include "../components/buildings/Tower.h"
#include "../entities/Enemy.h"
#include "../entities/Minion.h"
#include "../entities/Player.h"
#include "../rendering/TextRenderer.h"
#include "../systems/BuildingManager.h"
#include "../systems/EntityManager.h"
#include "../systems/ProjectileManager.h"
#include "Event.h"
#include "Window.h"

class Game {
 public:
  Game();
  ~Game();

  void run();

  void endGame();

 private:
  EventDispatcher m_eventDispatcher;
  Window m_window;
  Camera m_camera;

  std::shared_ptr<Player> m_player;
  TextRenderer m_textRenderer;
  EntityManager m_entityManager;

  ProjectileManager m_projectileManager;

  sf::Clock m_clock;

  Map::Rift m_map;

  std::vector<std::shared_ptr<Enemy>> m_enemies;

  std::vector<std::shared_ptr<Minion>> m_blueSideMinions;
  std::vector<std::shared_ptr<Minion>> m_redSideMinions;

  std::vector<std::shared_ptr<Tower>> m_blueSideTowers;
  std::vector<std::shared_ptr<Tower>> m_redSideTowers;

  void processSFMLEvents();
};
