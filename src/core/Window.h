#include <SFML/Graphics.hpp>

#include "Event.h"

class Window : public sf::RenderWindow {
 public:
  Window(EventDispatcher& dispatcher);

  ~Window();

  void confineCursorToWindow();

 private:
  EventDispatcher& m_eventDispatcher;

  void handleCursorOnEdge();
};