#pragma once

#include "SFML/Graphics.hpp"
#include "Event.h"

class Building {
public:
    Building(EventDispatcher& dispatcher, const std::string& texturePath, sf::Vector2f position, float scale = 1.0f);

    void onDraw(DrawEvent& event);

    sf::Vector2f getCenterPosition();
private:
    sf::Texture m_texture; 
    sf::Sprite m_sprite;  
    sf::Vector2f m_position;
};
