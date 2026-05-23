#pragma once

#include <SFML/Graphics.hpp>

#include "../core/Event.h"
#include "../entities/Player.h"

class AbilityBar {
 public:
  AbilityBar(EventDispatcher& dispatcher, const Player& player, const std::string& fontPath);
  ~AbilityBar();

 private:
  EventDispatcher& m_eventDispatcher;
  const Player& m_player;
  sf::Font m_font;
  sf::Text m_keyText;
  sf::Text m_cooldownText;

  void onDraw(DrawEvent& event);
  void drawSlot(sf::RenderWindow& window, const AbilityCooldownView& ability, sf::Vector2f position);
};
