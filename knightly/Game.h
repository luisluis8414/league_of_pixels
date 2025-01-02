#pragma once

#include <SFML/Graphics.hpp>
#include "Event.h"
#include "Enemy.h"
#include "Player.h"
#include "Map.h"

class Game {
public:
    Game();
    ~Game();

    void run(); 

    void endGame();

private:
    sf::RenderWindow m_window;
    EventDispatcher m_eventDispatcher;

    Map::Rift m_rift;

    std::vector<std::unique_ptr<Enemy>> m_enemies;
    std::vector<Entity*> m_entitiesToDestroy;

    void spawnEnemy(const std::string& texturePath, float x, float y);
    void cleanUp();
};
