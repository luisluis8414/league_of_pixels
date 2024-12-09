#pragma once

#include "SFML/Graphics.hpp"
#include "Event.h"

class Building {
public:
    Building(EventDispatcher& dispatcher, const std::string& texturePath, float x, float y, float scale = 1.0f);

    void onDraw(DrawEvent& event);

private:
    sf::Texture m_texture; 
    sf::Sprite m_sprite;  
};
