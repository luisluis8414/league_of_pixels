#include "Building.h"
#include "Game.h"
#include "Player.h"
#include <iostream>
#include "Enemy.h"
#include "CollisionSystem.h"

Game::Game()
    : m_window(sf::VideoMode(800, 800), "Knightly"), m_eventDispatcher() {
    m_eventDispatcher.subscribe<DestroyEntityEvent>(this, [this](DestroyEntityEvent& event) {
        m_entitiesToDestroy.push_back(event.GetEntity());
     });

}

Game::~Game() {
    if (m_window.isOpen()) {
        m_window.close();
    }
}

void Game::endGame() {
    if (m_window.isOpen()) {
        m_window.close();  
    }
    std::cout << "Game Over!" << std::endl;  
}

void Game::run() {
    const float targetFrameTime = 1.0f / 60.0f;  // 60fps and ticks
    int fps = 0;

    sf::Clock clock;

    sf::Clock fpsClock;

    CollisionSystem collisionSystem(m_eventDispatcher);

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
    spawnEnemy("resources/tiny_swords/Factions/Goblins/Troops/Torch/Red/Torch_Red.png", 100.f, 100.f);
    spawnEnemy("resources/tiny_swords/Factions/Goblins/Troops/TNT/Red/TNT_Red.png", 200.f, 200.f);
    
    Player player(m_eventDispatcher, 0.f, 0.f);

    sf::Event e;
    while (m_window.isOpen()) {
        sf::Time deltaTime = clock.restart();

        // process SFML events
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

        TickEvent tickEvent(deltaTime.asSeconds());
        m_eventDispatcher.emit(tickEvent);

        collisionSystem.update(player, m_enemies);

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

        if (m_entitiesToDestroy.size() > 0) {
            cleanUp();
        }

        // cap frame rate
        sf::Time frameEnd = sf::seconds(targetFrameTime) - clock.getElapsedTime();
        if (frameEnd > sf::Time::Zero) sf::sleep(frameEnd);
    }
}

void Game::spawnEnemy(const std::string& texturePath, float x, float y) {
    m_enemies.push_back(std::make_unique<Enemy>(m_eventDispatcher, texturePath, x, y));
}

void Game::cleanUp() {
    for (int i = 0; i < m_entitiesToDestroy.size(); ++i) {
        for (int j = 0; j < m_enemies.size(); ++j) {
            if (m_enemies[j].get() == m_entitiesToDestroy[i]) {
                m_enemies.erase(m_enemies.begin() + j);
                break;  
            }
        }
    }
    m_entitiesToDestroy.clear();
}




