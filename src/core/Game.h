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
#include "../systems/ProjectileManager.h"
#include "../systems/WorldManager.h"
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
  WorldManager m_worldManager;

  sf::Clock m_clock;

  Map::Rift m_map;

  void processSFMLEvents();
};
