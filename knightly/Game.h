#pragma once

#include <SFML/Graphics.hpp>
#include "Event.h"

class Game {
public:
    Game();
    ~Game();

    void run(); 

    void endGame();

private:
    sf::RenderWindow m_window;
    EventDispatcher m_eventDispatcher;
};
