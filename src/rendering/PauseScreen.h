#pragma once

#include <SFML/Graphics.hpp>

#include "../core/Event.h"
#include "../core/Window.h"

class PauseScreen {
 public:
  PauseScreen(EventDispatcher& dispatcher, Window& window);
  ~PauseScreen();

  bool isPaused() const {
    return m_paused;
  }

 private:
  EventDispatcher& m_eventDispatcher;
  Window& m_window;

  bool m_paused = false;

  sf::Font m_font;

  const sf::Texture& m_panelTexture;
  const sf::Texture& m_resumeTexture;
  const sf::Texture& m_resumePressedTexture;
  const sf::Texture& m_quitTexture;
  const sf::Texture& m_quitPressedTexture;

  sf::Sprite m_panel;
  sf::Sprite m_resumeButton;
  sf::Sprite m_quitButton;

  sf::Text m_titleText;
  sf::Text m_resumeText;
  sf::Text m_quitText;

  sf::RectangleShape m_dimmer;

  bool m_resumeHeld = false;
  bool m_quitHeld = false;

  void layout();
  void onDraw(DrawEvent& event);
  void onKeyPressed(KeyPressedEvent& event);
  void onLeftClick(MouseLeftClickEvent& event);
};
