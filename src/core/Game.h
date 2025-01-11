#pragma once

#include <SFML/Graphics.hpp>

#include "../components/Camera.h"
#include "../components/Map.h"
#include "../components/buildings/Building.h"
#include "../components/buildings/Tower.h"
#include "../entities/Enemy.h"
#include "../entities/Minion.h"
#include "../entities/Player.h"
#include "../rendering/TextRenderer.h"
#include "../systems/MovementManager.h"
#include "Event.h"

class Game {
 public:
  Game();
  ~Game();

  void run();

  void endGame();

 private:
  EventDispatcher m_eventDispatcher;
  sf::RenderWindow m_window;
  Camera m_camera;

  Player m_player;
  TextRenderer m_textRenderer;
  MovementManager m_movementManager;

  sf::Clock m_clock;

  Map::Rift m_map;

  std::vector<std::shared_ptr<Enemy>> m_enemies;
  std::vector<Entity*> m_entitiesToDestroy;

  void spawnEnemy(const std::string& texturePath, sf::Vector2f position);
  void cleanUp();

  void releaseCursor();
  void confineCursorToWindow();

  void handleCursorOnEdge();

  sf::Clock m_spawnClock;
  int m_minionsSpawned = 0;
  bool m_spawnCycleActive = false;

  std::vector<std::shared_ptr<Minion>> m_blueSideMinions;
  std::vector<std::shared_ptr<Minion>> m_redSideMinions;

  std::vector<std::shared_ptr<Tower>> m_blueSideTowers;
  std::vector<std::shared_ptr<Tower>> m_redSideTowers;

  std::vector<Minion*> m_minionsToDestroy;

  void spawnMinions(Building& spawn, Building& targetBuilding);
};
