#include "TextRenderer.h"

TextRenderer::TextRenderer(EventDispatcher& dispatcher, const std::string& fontPath)
    : m_eventDispatcher(dispatcher), m_text(m_font) {
  if (!m_font.openFromFile(fontPath)) {
    std::cerr << "Couldn't load font from: " << fontPath << std::endl;
    return;
  }
  m_text.setFont(m_font);
  m_text.setFillColor(sf::Color::Green);

  m_eventDispatcher.subscribe<DrawEvent>(
      this,
      [this](DrawEvent& event) {
        m_fps++;
        this->draw(event.getWindow());
      },
      RenderLayer::TEXT);

  m_eventDispatcher.subscribe<SecondEvent>(this, [this](SecondEvent& event) { m_fps = 0; });
}

void TextRenderer::setText(const std::string& text) {
  m_text.setString("FPS: " + std::to_string(m_fps));
}

void TextRenderer::draw(sf::RenderWindow& window) {
  sf::View originalView = window.getView();

  window.setView(window.getDefaultView());

  sf::Vector2u windowSize = window.getSize();
  sf::FloatRect textBounds = m_text.getLocalBounds();

  float xPos = static_cast<float>(windowSize.x) - textBounds.size.x - 15.0f;
  float yPos = 10.0f;

  m_text.setPosition({xPos, yPos});

  window.draw(m_text);

  window.setView(originalView);
}