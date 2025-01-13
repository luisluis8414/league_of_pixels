#pragma once

#include "../core/Event.h"

class Camera {
 public:
  Camera(EventDispatcher& dispatcher, sf::RenderWindow& window);

  ~Camera() = default;

  void centerCamera(sf::Vector2f playerPosition);

 private:
  sf::RenderWindow& m_window;

  void handleScroll(int x, int y, float delta);

  void handleCursorOnEdge(int edgeMask);
};