#pragma once

#include <SFML/Graphics.hpp>
#include "Event.h"
#include "Enemy.h"

class Game {
public:
    Game();
    ~Game();

    void run(); 

    void endGame();

private:
    sf::RenderWindow m_window;
    EventDispatcher m_eventDispatcher;

    std::vector<Enemy*> m_enemies;
    std::vector<Entity*> m_entitiesToDestroy;
    
    void spawnEnemy(const std::string& texturePath, float x, float y);
    void cleanUp();
};
