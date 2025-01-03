#include "TextRenderer.h"

TextRenderer::TextRenderer(EventDispatcher& dispatcher, const std::string& fontPath): m_eventDispatcher(dispatcher) {
    if (!m_font.loadFromFile(fontPath)) {
        std::cerr << "Couldn't load font from: " << fontPath << std::endl;
        return;
    }
    m_text.setFont(m_font);
    m_text.setCharacterSize(10);
    m_text.setFillColor(sf::Color::Green);
    m_text.setPosition(5.f, 10.f);

    m_eventDispatcher.subscribe<DrawEvent>(this, [this](DrawEvent& event) {
        draw(event.getWindow());
    }, RenderLayer::TEXT);
}

void TextRenderer::setText(const std::string& text) {
    m_text.setString(text);
}

void TextRenderer::draw(sf::RenderWindow& window) {
    window.draw(m_text);
}