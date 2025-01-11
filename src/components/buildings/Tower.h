#include "../../core/Event.h"
#include "Building.h"

class Tower : public Building {
 public:
  Tower(EventDispatcher& dispatcher,
        const std::string& towerTexturePath,
        const std::string& archerTexturePath,
        sf::Vector2f position,
        float health = 100,
        float scale = 1.0f);

 private:
  sf::Texture m_archerTexture;
  sf::Sprite m_archerSprite;
  float m_aaRange;

  void onDraw(DrawEvent& event);
};