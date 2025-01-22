#include "Game.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#include "Config.h"

Game::Game()
    : m_window(m_eventDispatcher),
      m_camera(m_eventDispatcher, m_window),
      m_player(std::make_shared<Player>(m_eventDispatcher,
                                        sf::Vector2f(600.f, 600.f),
                                        Config::Textures::Spells::Garen::Q,
                                        Config::Textures::Spells::Garen::W,
                                        Config::Textures::Spells::Garen::E,
                                        Config::Textures::Spells::Garen::R)),
      m_map(m_eventDispatcher),
      m_textRenderer(m_eventDispatcher, Config::Fonts::ARIAL),
      m_worldManager(m_eventDispatcher, m_player) {
}

Game::~Game() {
  if (m_window.isOpen()) {
    m_window.close();
  }
}

void Game::run() {
  m_eventDispatcher.emit(InitEvent());

  const float fpsTarget = 60.0f;
  const float targetFrameTime = 1.0f / fpsTarget;  // 60fps and ticks

  sf::Clock clock;
  sf::Clock secondsClock;

  while (m_window.isOpen()) {
    sf::Time deltaTime = clock.restart();

    // process SFML events
    processSFMLEvents();

    // game tick
    // pass deltaTime , hard coded value for debugging 0.016f
    m_eventDispatcher.emit(TickEvent(deltaTime.asSeconds()));

    // render frame
    m_window.clear();
    m_eventDispatcher.emit(DrawEvent(m_window));
    m_window.display();

    // emit event each second
    if (secondsClock.getElapsedTime().asSeconds() >= 1.0f) {
      m_eventDispatcher.emit(SecondsEvent());
      secondsClock.restart();
    }

    m_eventDispatcher.emit(CleanUpEvent());

    // cap frame rate
    sf::Time frameEnd = sf::seconds(targetFrameTime) - clock.getElapsedTime();
    if (frameEnd > sf::Time::Zero) sf::sleep(frameEnd);
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
      m_window.confineCursorToWindow();
      sf::View newView(sf::FloatRect({0.f, 0.f}, {(float)resizedEvent->size.x, (float)resizedEvent->size.y}));
      m_window.setView(newView);
    } else if (const sf::Event::MouseWheelScrolled* resizedEvent = event->getIf<sf::Event::MouseWheelScrolled>()) {
      m_camera.handleScroll(resizedEvent->position.x, resizedEvent->position.y, resizedEvent->delta);
    }
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
    m_camera.centerCamera(m_player->getPosition());
  };
}

void Game::endGame() {
  if (m_window.isOpen()) {
    m_window.close();
  }
  std::cout << "Game Over!" << std::endl;
}
