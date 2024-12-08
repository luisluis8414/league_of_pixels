#include "Building.h"
#include <iostream>

// constructor: loads the texture and sets up the sprite
Building::Building(const std::string& texturePath, float x, float y, float scale) {
    if (!m_texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load " << texturePath << std::endl;
    }
    m_texture.setSmooth(false);        // disable smoothing for pixel-perfect rendering
    m_sprite.setTexture(m_texture);   // assign the texture to the sprite
    m_sprite.setPosition(x, y);       // position the building
    m_sprite.setScale(scale, scale);  // scale the building
}

// draw the building on the window
void Building::draw(sf::RenderWindow& window) {
    window.draw(m_sprite);
}
