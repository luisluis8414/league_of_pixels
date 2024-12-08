#include "Building.h"
#include "Game.h"

Game::Game()
    : m_window(sf::VideoMode(800, 800), "My game") {
}

Game::~Game() {
    if (m_window.isOpen()) {
        m_window.close();
    }
}

void Game::run() {
    Building building("resources/tiny_swords/Factions/Knights/Buildings/Castle/Castle_Blue.png", 200, 200, 1.2);

    sf::Event e;

    while (m_window.isOpen()) {
        while (m_window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                m_window.close();
            }
        }

        m_window.clear(); // clear the screen
        // add drawing/rendering logic here
        building.draw(m_window);

        m_window.display(); // present the frame
    }
}
