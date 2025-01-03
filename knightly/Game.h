#pragma once

#include <SFML/Graphics.hpp>
#include "Event.h"
#include "Enemy.h"
#include "Player.h"
#include "Map.h"
#include "TextRenderer.h"
#include "CollisionSystem.h"

class Game {
public:
    Game();
    ~Game();

    void run(); 

    void endGame();

private:
    sf::RenderWindow m_window;
    EventDispatcher m_eventDispatcher;

    Player m_player;
    TextRenderer m_textRenderer;
    CollisionSystem m_collisionSystem;

    Map::Rift m_map;

    std::vector<std::unique_ptr<Enemy>> m_enemies;
    std::vector<Entity*> m_entitiesToDestroy;

    void spawnEnemy(const std::string& texturePath, float x, float y);
    void cleanUp();
};
