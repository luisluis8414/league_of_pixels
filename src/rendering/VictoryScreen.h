#pragma once

#include <SFML/Graphics.hpp>

#include "../core/Event.h"
#include "../core/Window.h"

class VictoryScreen {
 public:
  VictoryScreen(EventDispatcher& dispatcher, Window& window);
  ~VictoryScreen();

 private:
  EventDispatcher& m_eventDispatcher;
  Window& m_window;

  bool m_visible = false;

  sf::Font m_font;

  const sf::Texture& m_panelTexture;
  const sf::Texture& m_exitTexture;

  sf::Sprite m_panel;
  sf::Sprite m_exitButton;

  sf::Text m_titleText;
  sf::Text m_exitText;

  sf::RectangleShape m_dimmer;

  void layout();
  void onDraw(DrawEvent& event);
  void onLeftClick(MouseLeftClickEvent& event);
  void onGameOver(GameOverEvent& event);
};
