#include "TextRenderer.h"

TextRenderer::TextRenderer(EventDispatcher& dispatcher, const std::string& fontPath)
    : m_eventDispatcher(dispatcher), m_fpsText(m_font) {
  if (!m_font.openFromFile(fontPath)) {
    std::cerr << "Couldn't load font from: " << fontPath << std::endl;
    return;
  }
  m_fpsText.setFont(m_font);
  m_fpsText.setFillColor(sf::Color::Green);
  m_fpsText.setString("FPS: 60");

  m_eventDispatcher.subscribe<DrawEvent>(
      this,
      [this](DrawEvent& event) {
        m_fps++;
        this->draw(event.getWindow());
      },
      RenderLayer::TEXT);

  m_eventDispatcher.subscribe<SecondsEvent>(this, [this](SecondsEvent& event) {
    m_fpsText.setString("FPS: " + std::to_string(m_fps));
    m_fps = 0;
  });
}

void TextRenderer::draw(sf::RenderWindow& window) {
  sf::View originalView = window.getView();

  window.setView(window.getDefaultView());

  sf::Vector2u windowSize = window.getSize();
  sf::FloatRect textBounds = m_fpsText.getLocalBounds();

  float xPos = static_cast<float>(windowSize.x) - textBounds.size.x - 15.0f;
  float yPos = 10.0f;

  m_fpsText.setPosition({xPos, yPos});

  window.draw(m_fpsText);

  window.setView(originalView);
}