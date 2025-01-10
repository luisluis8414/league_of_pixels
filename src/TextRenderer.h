#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "Event.h"

class TextRenderer {
 public:
  TextRenderer(EventDispatcher& dispatcher, const std::string& fontPath);

  void setText(const std::string& text);

  void draw(sf::RenderWindow& window);

 private:
  EventDispatcher& m_eventDispatcher;

  sf::Font m_font;
  sf::Text m_text;
};
