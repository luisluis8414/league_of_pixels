#pragma once

#include "SFML/Graphics.hpp"

class Building {
public:
    Building(const std::string& texturePath, float x, float y, float scale = 1.0f);

    void draw(sf::RenderWindow& window);

private:
    sf::Texture m_texture; 
    sf::Sprite m_sprite;  
};
