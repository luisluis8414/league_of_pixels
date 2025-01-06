#pragma once

#include <SFML/Graphics.hpp>
#include "Event.h"
#include "Enemy.h"
#include "Player.h"
#include "Map.h"
#include "TextRenderer.h"
#include "MovementManager.h"
#include "Camera.h"
#include "Minion.h"
#include "Building.h"

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

    std::vector<Minion*> m_minionsToDestroy;

    void spawnMinions(Building& spawn, Building& targetBuilding);
};
