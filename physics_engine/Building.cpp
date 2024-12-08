#include "Building.h"
#include <iostream>

Building::Building(const std::string& texturePath, float x, float y, float scale) {
    if (!m_texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load " << texturePath << std::endl;
    }
    m_texture.setSmooth(false);        
    m_sprite.setTexture(m_texture);   
    m_sprite.setPosition(x, y);      
    m_sprite.setScale(scale, scale); 
}


void Building::draw(sf::RenderWindow& window) {
    window.draw(m_sprite);
}
