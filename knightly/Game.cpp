#include "Building.h"
#include "Game.h"
#include "Player.h"
#include <iostream>
#include "Enemy.h"
#include "MovementManager.h"
#include "Config.h"
#include "Minion.h"
#include <windows.h>

Game::Game()
	:	m_window(sf::VideoMode::getDesktopMode(), 
		Config::Window::TITLE, sf::Style::Fullscreen), 
		m_eventDispatcher(), 
		m_camera(m_eventDispatcher, m_window), 
		m_player(m_eventDispatcher, { 600.f, 600.f }, Config::Textures::Spells::Garen::Q, Config::Textures::Spells::Garen::W),
		m_clock(),
		m_map(m_eventDispatcher), m_textRenderer(m_eventDispatcher, Config::Fonts::ARIAL), 
		m_movementManager(m_eventDispatcher, m_player, m_enemies, m_blueSideMinions, m_redSideMinions) {

	sf::Image icon;
	if (!icon.loadFromFile(Config::Window::ICON_PATH)) {
		std::cerr << "failed to load icon!" << std::endl;
		return;
	}

	m_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	
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
	confineCursorToWindow();

	const float fpsTarget = 60.0f;
	const float targetFrameTime = 1.0f / fpsTarget;  // 60fps and ticks
	int fps = 0;

	sf::Clock clock;
	sf::Clock fpsClock;
	sf::Clock minionSpawnClock;


	Building castleBlue(m_eventDispatcher, Config::Textures::Buildings::BLUE_SIDE_NEXUS, { 300.f, 400.f }, 1.f);
	Building castleRed(m_eventDispatcher, Config::Textures::Buildings::RED_SIDE_NEXUS, { 3500.f, 400.f }, 1.f);

	Building blueSideT3Tower1(m_eventDispatcher, Config::Textures::Buildings::T3_BLUE_SIDE_TOWER, { 700.f, 300.f }, 1.f);
	Building blueSideT3Tower2(m_eventDispatcher, Config::Textures::Buildings::T3_BLUE_SIDE_TOWER, { 700.f, 500.f }, 1.f);
	Building blueSideT2Tower(m_eventDispatcher, Config::Textures::Buildings::T3_BLUE_SIDE_TOWER, { 1100.f, 400.f }, 1.f);
	Building blueSideT1Tower(m_eventDispatcher, Config::Textures::Buildings::T3_BLUE_SIDE_TOWER, { 1600.f, 400.f }, 1.f);

	Building redSideT3Tower1(m_eventDispatcher, Config::Textures::Buildings::T3_RED_SIDE_TOWER, { 3300.f, 300.f }, 1.f);
	Building redSideT3Tower2(m_eventDispatcher, Config::Textures::Buildings::T3_RED_SIDE_TOWER, { 3300.f, 500.f }, 1.f);
	Building redSideT2Tower(m_eventDispatcher, Config::Textures::Buildings::T3_RED_SIDE_TOWER, { 2900.f, 400.f }, 1.f);
	Building redSideT1Tower(m_eventDispatcher, Config::Textures::Buildings::T3_RED_SIDE_TOWER, { 2400.f, 400.f }, 1.f);

	//spawnEnemy(Config::Textures::Troops::TORCH_RED, { 200.f , 200.f });
	//spawnEnemy(Config::Textures::Troops::TNT_RED, { 200.f , 300.f });

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
					sf::Vector2f worldPosition = m_window.mapPixelToCoords({ e.mouseButton.x, e.mouseButton.y });
					MouseRightClickEvent event(worldPosition);
					m_eventDispatcher.emit(event);
				}
			}

			if (e.type == sf::Event::KeyPressed) {
				sf::Vector2i mousePosition =  sf::Mouse::getPosition();
				sf::Vector2f mouseWorldPosition = m_window.mapPixelToCoords(mousePosition);
				KeyPressedEvent keyEvent(e.key, mouseWorldPosition);
				m_eventDispatcher.emit(keyEvent);
			}

			if (e.type == sf::Event::Resized) {
				confineCursorToWindow();
				sf::View newView(sf::FloatRect(0.f, 0.f, (float)e.size.width, (float)e.size.height));
				m_window.setView(newView);
			}

			if (e.type == sf::Event::MouseWheelScrolled)
			{
				ScrollEvent scrollEvent(e.mouseWheelScroll.x, e.mouseWheelScroll.y,e.mouseWheelScroll.delta);
				m_eventDispatcher.emit(scrollEvent);
			}
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			CenterCameraEvent centerCameraEvent(m_player.getPosition());
			m_eventDispatcher.emit(centerCameraEvent);
		};

		/*if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			sf::Vector2i pixelPos = sf::Mouse::getPosition(m_window);
			sf::Vector2f worldPos = m_window.mapPixelToCoords(pixelPos);

			MouseRightClickEvent event(worldPos);
			m_eventDispatcher.emit(event);
		}
	*/
		if (minionSpawnClock.getElapsedTime().asSeconds() >= Config::Minions::SPAWN_TIMER || m_spawnCycleActive) {
			spawnMinions(castleBlue, castleRed);
			minionSpawnClock.restart();
		}

		handleCursorOnEdge();

		// pass deltaTime , hard coded value for debugging
		TickEvent tickEvent(0.016f);
		m_eventDispatcher.emit(tickEvent);

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

	releaseCursor();
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
		for (int j = 0; j < m_redSideMinions.size(); ++j) {
			if (m_redSideMinions[j].get() == m_entitiesToDestroy[i]) {
				m_redSideMinions.erase(m_redSideMinions.begin() + j);
				break;
			}
		}
		for (int j = 0; j < m_blueSideMinions.size(); ++j) {
			if (m_blueSideMinions[j].get() == m_entitiesToDestroy[i]) {
				m_blueSideMinions.erase(m_blueSideMinions.begin() + j);
				break;
			}
		}
	}
	m_entitiesToDestroy.clear();
}

void Game::confineCursorToWindow() {
	sf::Vector2i windowPos = m_window.getPosition();
	sf::Vector2u windowSize = m_window.getSize();

	RECT rect = {
		static_cast<long>(windowPos.x),
		static_cast<long>(windowPos.y),
		static_cast<long>(windowPos.x + windowSize.x),
		static_cast<long>(windowPos.y + windowSize.y)
	};

	ClipCursor(&rect); 
}

void Game::releaseCursor() {
	ClipCursor(nullptr); 
}

void Game::handleCursorOnEdge() {
	int threshold = 10;

	sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
	sf::Vector2u windowSize = m_window.getSize();

	int edges = MouseEdge::None;

	if (mousePos.x <= threshold) edges |= MouseEdge::Left;
	if (mousePos.x >= static_cast<int>(windowSize.x) - threshold) edges |= MouseEdge::Right;
	if (mousePos.y <= threshold) edges |= MouseEdge::Top;
	if (mousePos.y >= static_cast<int>(windowSize.y) - threshold) edges |= MouseEdge::Bottom;

	if (edges != MouseEdge::None) {
		MouseOnEdgeEvent edgeEvent(edges);
		m_eventDispatcher.emit(edgeEvent);
	}
}

void Game::spawnMinions(Building& blueSideNexus, Building& redSideNexus) {
	if (!m_spawnCycleActive) {
		m_spawnCycleActive = true;
		m_spawnClock.restart();
		m_minionsSpawned = 0;
	}

	if (m_spawnCycleActive) {
		float elapsed = m_spawnClock.getElapsedTime().asSeconds();

		if (elapsed >= 3.f && m_minionsSpawned < 3) {
			m_blueSideMinions.push_back(std::make_unique<Minion>(m_eventDispatcher, Config::Textures::Troops::MINIONS_BLUE, blueSideNexus.getCenterPosition(), redSideNexus.getCenterPosition()));
			m_redSideMinions.push_back(std::make_unique<Minion>(m_eventDispatcher, Config::Textures::Troops::MINIONS_RED, redSideNexus.getCenterPosition(), blueSideNexus.getCenterPosition()));
			
			m_spawnClock.restart();
			m_minionsSpawned++;
		}

		if (m_minionsSpawned >= 3) {
			m_spawnCycleActive = false;
		}
	}
}



