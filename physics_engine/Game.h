#pragma once

#include <SFML/Graphics.hpp>

class Game {
public:
    Game();
    ~Game();

    void run(); 

private:
    sf::RenderWindow m_window;
};
