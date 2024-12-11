#include "Building.h"
#include "Game.h"
#include "Player.h"
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
    const float targetFrameTime = 1.0f / 60.0f;  // 60fps and ticks
    int fps = 0;

    sf::Clock clock;

    sf::Clock fpsClock;

    sf::Font font;
    if (!font.loadFromFile("resources/fonts/arial.ttf")) {
        std::cerr << "Couldn't load font arial at: resources/fonts/arial.ttf!" << std::endl;
        return;
    }

    sf::Text fpsText;
    fpsText.setFont(font);
    fpsText.setCharacterSize(10);
    fpsText.setFillColor(sf::Color::Green);
    fpsText.setPosition(5.f, 10.f);

    Building building(m_eventDispatcher, "resources/tiny_swords/Factions/Knights/Buildings/Castle/Castle_Blue.png", 200, 200, 1.2f);
    Player player(m_eventDispatcher);
    player.setPosition(300.f, 300.f);
    player.setAnimation(AnimationState::Idle);

    sf::Event e;
    while (m_window.isOpen()) {
        sf::Time deltaTime = clock.restart();

        // process events
        while (m_window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                m_window.close();
            }

            if (e.type == sf::Event::KeyPressed) {
                KeyPressedEvent keyEvent(e.key);
                m_eventDispatcher.emit(keyEvent);
            }

            if (e.type == sf::Event::Resized) {
                sf::View newView(sf::FloatRect(0.f, 0.f, (float)e.size.width, (float)e.size.height));
                m_window.setView(newView);
            }
        }

        TickEvent tickEvent(targetFrameTime);
        m_eventDispatcher.emit(tickEvent);

        // render frame
        m_window.clear();
        DrawEvent drawEvent(m_window);
        m_eventDispatcher.emit(drawEvent);
        m_window.draw(fpsText);
        m_window.display();
        fps++;

        // update fps/ticks counters every second
        if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
            fpsText.setString("FPS: " + std::to_string(fps));
            fps = 0;
            fpsClock.restart();
        }

        // cap frame rate
        sf::Time frameEnd = sf::seconds(targetFrameTime) - clock.getElapsedTime();
        if (frameEnd > sf::Time::Zero) sf::sleep(frameEnd);
    }
}


