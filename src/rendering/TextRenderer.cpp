#include "TextRenderer.h"

namespace {
std::string formatTimer(int seconds) {
  int minutes = seconds / 60;
  int remaining = seconds % 60;
  std::string mm = (minutes < 10 ? "0" : "") + std::to_string(minutes);
  std::string ss = (remaining < 10 ? "0" : "") + std::to_string(remaining);
  return mm + ":" + ss;
}
}  // namespace

TextRenderer::TextRenderer(EventDispatcher& dispatcher, const std::string& fontPath)
    : m_eventDispatcher(dispatcher), m_fpsText(m_font), m_timerText(m_font) {
  if (!m_font.openFromFile(fontPath)) {
    std::cerr << "Couldn't load font from: " << fontPath << std::endl;
    return;
  }
  m_fpsText.setFont(m_font);
  m_fpsText.setFillColor(sf::Color::Green);
  m_fpsText.setString("FPS: 60");

  m_timerText.setFont(m_font);
  m_timerText.setFillColor(sf::Color::White);
  m_timerText.setString(formatTimer(0));

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
    m_elapsedSeconds++;
    m_timerText.setString(formatTimer(m_elapsedSeconds));
  });
}

void TextRenderer::draw(sf::RenderWindow& window) {
  sf::View originalView = window.getView();

  window.setView(window.getDefaultView());

  sf::Vector2u windowSize = window.getSize();

  sf::FloatRect fpsBounds = m_fpsText.getLocalBounds();
  float fpsX = static_cast<float>(windowSize.x) - fpsBounds.size.x - 15.0f;
  float yPos = 10.0f;
  m_fpsText.setPosition({fpsX, yPos});

  sf::FloatRect timerBounds = m_timerText.getLocalBounds();
  float timerX = fpsX - timerBounds.size.x - 20.0f;
  m_timerText.setPosition({timerX, yPos});

  window.draw(m_fpsText);
  window.draw(m_timerText);

  window.setView(originalView);
}