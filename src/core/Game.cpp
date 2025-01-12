#include "Game.h"

// todo: replace ClipCursor with own thread
#include <windows.h>

#include <iostream>

#include "Config.h"

Game::Game()
    : m_window(sf::VideoMode::getFullscreenModes().at(1), Config::Window::TITLE, sf::State::Fullscreen),
      m_eventDispatcher(),
      m_camera(m_eventDispatcher, m_window),
      m_player(m_eventDispatcher,
               {600.f, 600.f},
               Config::Textures::Spells::Garen::Q,
               Config::Textures::Spells::Garen::W,
               Config::Textures::Spells::Garen::E,
               Config::Textures::Spells::Garen::R),
      m_clock(),
      m_map(m_eventDispatcher),
      m_textRenderer(m_eventDispatcher, Config::Fonts::ARIAL),
      m_movementManager(m_eventDispatcher, m_player, m_enemies, m_blueSideMinions, m_redSideMinions, m_blueSideTowers),
      m_buildingManager(m_eventDispatcher) {
  sf::Image icon;
  if (!icon.loadFromFile(Config::Window::ICON_PATH)) {
    std::cerr << "failed to load icon!" << std::endl;
    return;
  }

  m_window.setVerticalSyncEnabled(true);
  m_window.setFramerateLimit(60);

  m_window.setIcon(icon);
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

  sf::Clock clock;
  sf::Clock secondClock;
  sf::Clock minionSpawnClock;

  m_eventDispatcher.emit(InitEvent());

  Building nexusBlue(m_eventDispatcher, Config::Textures::Buildings::BLUE_SIDE_NEXUS, {300.f, 400.f});
  Building nexusRed(m_eventDispatcher, Config::Textures::Buildings::RED_SIDE_NEXUS, {3500.f, 400.f});

  while (m_window.isOpen()) {
    sf::Time deltaTime = clock.restart();

    // process SFML events
    processSFMLEvents();

    // move screen when cursor on edge
    handleCursorOnEdge();

    if (minionSpawnClock.getElapsedTime().asSeconds() >= Config::Minions::SPAWN_TIMER || m_spawnCycleActive) {
      spawnMinions(nexusBlue, nexusRed);
      minionSpawnClock.restart();
    }

    // game tick
    // pass deltaTime , hard coded value for debugging
    m_eventDispatcher.emit(TickEvent(0.016f));

    // render frame
    m_window.clear();
    m_eventDispatcher.emit(DrawEvent(m_window));
    m_window.display();

    // emit each second event
    if (secondClock.getElapsedTime().asSeconds() >= 1.0f) {
      m_eventDispatcher.emit(SecondEvent());
      secondClock.restart();
    }

    m_eventDispatcher.emit(CleanUpEvent());

    // cap frame rate
    sf::Time frameEnd = sf::seconds(targetFrameTime) - clock.getElapsedTime();
    if (frameEnd > sf::Time::Zero) sf::sleep(frameEnd);
  }
  releaseCursor();
}

void Game::confineCursorToWindow() {
  sf::Vector2i windowPos = m_window.getPosition();
  sf::Vector2u windowSize = m_window.getSize();

  RECT rect = {static_cast<long>(windowPos.x),
               static_cast<long>(windowPos.y),
               static_cast<long>(windowPos.x + windowSize.x),
               static_cast<long>(windowPos.y + windowSize.y)};

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
      m_blueSideMinions.push_back(std::make_shared<Minion>(m_eventDispatcher,
                                                           Config::Textures::Troops::MINIONS_BLUE,
                                                           blueSideNexus.getCenterPosition(),
                                                           redSideNexus.getCenterPosition()));
      m_redSideMinions.push_back(std::make_shared<Minion>(m_eventDispatcher,
                                                          Config::Textures::Troops::MINIONS_RED,
                                                          redSideNexus.getCenterPosition(),
                                                          blueSideNexus.getCenterPosition()));

      m_spawnClock.restart();
      m_minionsSpawned++;
    }

    if (m_minionsSpawned >= 3) {
      m_spawnCycleActive = false;
    }
  }
}

void Game::processSFMLEvents() {
  while (const std::optional event = m_window.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      m_window.close();
    } else if (const sf::Event::MouseButtonPressed* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
      sf::Vector2f worldPosition = m_window.mapPixelToCoords({mouseButton->position.x, mouseButton->position.y});
      MouseRightClickEvent event(worldPosition);
      m_eventDispatcher.emit(event);
    } else if (const sf::Event::KeyPressed* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      sf::Vector2i mousePosition = sf::Mouse::getPosition();
      sf::Vector2f mouseWorldPosition = m_window.mapPixelToCoords(mousePosition);
      KeyPressedEvent keyEvent(keyPressed->code, mouseWorldPosition);
      m_eventDispatcher.emit(keyEvent);
    } else if (const sf::Event::Resized* resizedEvent = event->getIf<sf::Event::Resized>()) {
      confineCursorToWindow();
      sf::View newView(sf::FloatRect({0.f, 0.f}, {(float)resizedEvent->size.x, (float)resizedEvent->size.y}));
      m_window.setView(newView);
    } else if (const sf::Event::MouseWheelScrolled* resizedEvent = event->getIf<sf::Event::MouseWheelScrolled>()) {
      ScrollEvent scrollEvent(resizedEvent->position.x, resizedEvent->position.y, resizedEvent->delta);
      m_eventDispatcher.emit(scrollEvent);
    }
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
    CenterCameraEvent centerCameraEvent(m_player.getPosition());
    m_eventDispatcher.emit(centerCameraEvent);
  };
}