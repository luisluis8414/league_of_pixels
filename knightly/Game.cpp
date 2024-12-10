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
    const float targetFrameTime = 1.0f / 60.0f;
    sf::Clock clock;
    sf::Time accumulatedTime;

    sf::Clock fpsClock;
    int ticks = 0;
    int fps = 0;

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

    sf::Text updatesText;
    updatesText.setFont(font);
    updatesText.setCharacterSize(10);
    updatesText.setFillColor(sf::Color::Yellow);
    updatesText.setPosition(5.f, 20.f);

    Building building(m_eventDispatcher, "resources/tiny_swords/Factions/Knights/Buildings/Castle/Castle_Blue.png", 200, 200, 1.2);
    Player player(m_eventDispatcher);
    player.setPosition(300.f, 300.f);
    player.setAnimation(AnimationState::Idle);

    sf::Event e;
    while (m_window.isOpen()) {
        sf::Time timeSinceLastFrame = clock.restart();
        accumulatedTime = std::min((accumulatedTime + timeSinceLastFrame), sf::seconds(0.2f));

        // process events
        while (m_window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                m_window.close();
            }

            if (e.type == sf::Event::KeyPressed) {
                KeyPressedEvent keyEvent(e.key, accumulatedTime.asSeconds());
                m_eventDispatcher.emit(keyEvent);
            }
        }

        // update logic if enough time has passed
        while (accumulatedTime.asSeconds() >= targetFrameTime) {
            UpdateEvent updateEvent(targetFrameTime);
            m_eventDispatcher.emit(updateEvent);
            accumulatedTime -= sf::seconds(targetFrameTime);

            ticks++;
        }

        // log updates pro sekunde
        if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
            fpsText.setString("FPS: " + std::to_string(fps));
            updatesText.setString("Ticks: " + std::to_string(ticks));

            ticks = 0;
            fps = 0;
            fpsClock.restart();
        }

        m_window.clear();
        DrawEvent drawEvent(m_window);
        m_eventDispatcher.emit(drawEvent);

        m_window.draw(fpsText);
        m_window.draw(updatesText);

        m_window.display();
        fps++;

        sf::sleep(sf::seconds(targetFrameTime) - clock.getElapsedTime());
    }
}


