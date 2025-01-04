#include "Building.h"
#include "Game.h"
#include "Player.h"
#include <iostream>
#include "Enemy.h"
#include "CollisionSystem.h"
#include "Config.h"
#include "Minion.h"

Game::Game()
	: m_window(sf::VideoMode(Config::Window::WIDTH, Config::Window::HEIGHT), Config::Window::TITLE), m_eventDispatcher(), m_player(m_eventDispatcher, { 600.f, 600.f }), m_map(m_eventDispatcher), m_textRenderer(m_eventDispatcher, Config::Fonts::ARIAL), m_collisionSystem(m_eventDispatcher, m_player, m_enemies) {

	m_eventDispatcher.subscribe<DestroyEntityEvent>(this, [this](DestroyEntityEvent& event) {
		m_entitiesToDestroy.push_back(event.getEntity());
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
	const float fpsTarget = 60.0f;
	const float targetFrameTime = 1.0f / fpsTarget;  // 60fps and ticks
	int fps = 0;

	sf::Clock clock;
	sf::Clock fpsClock;


	Building caslteBlue(m_eventDispatcher, Config::Textures::Buildings::CASTLE_BLUE, 200, 400, 1.f);
	Building CastleRed(m_eventDispatcher, Config::Textures::Buildings::CASTLE_RED, 1400, 400, 1.f);
	spawnEnemy(Config::Textures::Troops::TORCH_RED, { 200.f , 200.f });
	spawnEnemy(Config::Textures::Troops::TNT_RED, { 200.f , 300.f });
	Minion minion(m_eventDispatcher, {400.f, 400.f});

	sf::Event e;
	while (m_window.isOpen()) {
		sf::Time deltaTime = clock.restart();

		// process SFML events
		while (m_window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				m_window.close();
			}

			if (e.type == sf::Event::MouseButtonPressed) {
				if (e.mouseButton.button == sf::Mouse::Right) {
					sf::Vector2 worldPosition = m_window.mapPixelToCoords({ e.mouseButton.x, e.mouseButton.y });
					MouseRightClickEvent event(worldPosition);
					m_eventDispatcher.emit(event);
				}
				else if (e.mouseButton.button == sf::Mouse::Left) {
					std::cout << "Right mouse button clicked at ("
						<< e.mouseButton.x << ", "
						<< e.mouseButton.y << ")\n";
				}
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

		// pass deltaTime , hard coded value for debugging
		TickEvent tickEvent(0.016f);
		m_eventDispatcher.emit(tickEvent);

		m_collisionSystem.update();

		// render frame
		m_window.clear();
		DrawEvent drawEvent(m_window);
		m_eventDispatcher.emit(drawEvent);
		m_window.display();

		fps++;

		// update fps/ticks counters every second
		if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
			m_textRenderer.setText("FPS: " + std::to_string(fps));
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

void Game::spawnEnemy(const std::string& texturePath, sf::Vector2f position) {
	m_enemies.push_back(std::make_unique<Enemy>(m_eventDispatcher, texturePath, position));
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




