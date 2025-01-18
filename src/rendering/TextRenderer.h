#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "../core/Event.h"

class TextRenderer {
 public:
  TextRenderer(EventDispatcher& dispatcher, const std::string& fontPath);

  void draw(sf::RenderWindow& window);

 private:
  EventDispatcher& m_eventDispatcher;

  sf::Font m_font;

  int m_fps = 0;
  sf::Text m_fpsText;
};
