#include "Window.h"

// todo: replace ClipCursor with own thread
#include <windows.h>

#include <iostream>

#include "Config.h"

Window::Window(EventDispatcher& dispatcher)
    : m_eventDispatcher(dispatcher),
      sf::RenderWindow(sf::VideoMode::getFullscreenModes().at(1), Config::Window::TITLE, sf::State::Fullscreen) {
  sf::Image icon;
  if (!icon.loadFromFile(Config::Window::ICON_PATH)) {
    std::cerr << "failed to load icon!" << std::endl;
    return;
  }

  setVerticalSyncEnabled(true);
  setFramerateLimit(60);

  setIcon(icon);

  m_eventDispatcher.subscribe<InitEvent>(this, [this](InitEvent& event) { this->confineCursorToWindow(); });

  m_eventDispatcher.subscribe<TickEvent>(this, [this](TickEvent& event) { this->handleCursorOnEdge(); });
}

Window::~Window() {
  ClipCursor(nullptr);
}

void Window::confineCursorToWindow() {
  sf::Vector2i windowPos = getPosition();
  sf::Vector2u windowSize = getSize();

  RECT rect = {static_cast<long>(windowPos.x),
               static_cast<long>(windowPos.y),
               static_cast<long>(windowPos.x + windowSize.x),
               static_cast<long>(windowPos.y + windowSize.y)};

  ClipCursor(&rect);
}

void Window::handleCursorOnEdge() {
  int threshold = 10;

  sf::Vector2i mousePos = sf::Mouse::getPosition();
  sf::Vector2u windowSize = getSize();

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