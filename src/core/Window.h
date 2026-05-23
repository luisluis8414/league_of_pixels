#pragma once

#include <SFML/Graphics.hpp>

#include <optional>

#include "Event.h"

class Window : public sf::RenderWindow {
 public:
  Window(EventDispatcher& dispatcher);

  ~Window();

  void confineCursorToWindow();

 private:
  EventDispatcher& m_eventDispatcher;
  std::optional<sf::Cursor> m_cursor;

  void handleCursorOnEdge();
};
