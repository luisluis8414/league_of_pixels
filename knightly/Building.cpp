#include "Building.h"
#include <iostream>

Building::Building(EventDispatcher& dispatcher, const std::string& texturePath, float x, float y, float scale) {
    if (!m_texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load " << texturePath << std::endl;
    }

    m_texture.setSmooth(false);        
    m_sprite.setTexture(m_texture);   
    m_sprite.setPosition(x, y);      
    m_sprite.setScale(scale, scale); 

    dispatcher.subscribe<DrawEvent>(this, [this](DrawEvent& event) {
        this->onDraw(event);
        });
}


void Building::onDraw(DrawEvent& event) {
    event.getWindow().draw(m_sprite);
}
