#pragma once

#include "../core/Event.h"

class Camera {
 public:
  Camera(EventDispatcher& dispatcher, sf::RenderWindow& window);

  ~Camera() = default;

  void centerCamera(sf::Vector2f playerPosition);

  void handleScroll(int x, int y, float delta);

 private:
  sf::RenderWindow& m_window;

  void handleCursorOnEdge(int edgeMask);
};