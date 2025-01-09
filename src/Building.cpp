#include "Building.h"
#include <iostream>

Building::Building(EventDispatcher& dispatcher, const std::string& texturePath, sf::Vector2f position, float scale): m_position(position), m_sprite(m_texture) {
    if (!m_texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load " << texturePath << std::endl;
    }

    m_texture.setSmooth(false);        
    m_sprite.setTexture(m_texture);   
    m_sprite.setPosition(position);      
    m_sprite.setScale({ scale, scale });

    dispatcher.subscribe<DrawEvent>(this, [this](DrawEvent& event) {
        this->onDraw(event);
        });
}


void Building::onDraw(DrawEvent& event) {
    event.getWindow().draw(m_sprite);
}

sf::Vector2f Building::getCenterPosition() {
    sf::FloatRect bounds = m_sprite.getGlobalBounds();

    sf::Vector2f center;
    center.x = bounds.position.x+ bounds.size.x / 2.0f; 
    center.y = bounds.position.y + bounds.size.y / 2.0f;

    return center;
}

