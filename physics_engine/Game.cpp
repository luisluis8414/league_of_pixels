#include "Building.h"
#include "Game.h"
#include "Player.h"
#include "Event.h"
#include <iostream>

Game::Game()
    : m_window(sf::VideoMode(800, 800), "Knightly") {
}

Game::~Game() {
    if (m_window.isOpen()) {
        m_window.close();
    }
}

void Game::run() {
    sf::Clock clock;

    Building building(m_eventDispatcher,"resources/tiny_swords/Factions/Knights/Buildings/Castle/Castle_Blue.png", 200, 200, 1.2);

    Player player(m_eventDispatcher);

    player.setPosition(300.f, 300.f);

    player.setAnimation(AnimationState::Idle);

    sf::Event e;

    while (m_window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        while (m_window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                m_window.close();
            }
        }

        player.handleInput(deltaTime);
        player.update(deltaTime);
            
        m_window.clear(); 
        DrawEvent drawEvent(m_window);
        m_eventDispatcher.emit(drawEvent);

        m_window.display(); 
    }
}

