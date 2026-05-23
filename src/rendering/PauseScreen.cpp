#include "PauseScreen.h"

#include <iostream>

#include "../core/AssetManager.h"
#include "../core/Config.h"

namespace {
const std::string kPanelPath = "resources/tiny_swords/UI/Banners/Banner_Vertical.png";
const std::string kResumePath = "resources/tiny_swords/UI/Buttons/Button_Blue.png";
const std::string kResumePressedPath = "resources/tiny_swords/UI/Buttons/Button_Blue_Pressed.png";
const std::string kQuitPath = "resources/tiny_swords/UI/Buttons/Button_Red.png";
const std::string kQuitPressedPath = "resources/tiny_swords/UI/Buttons/Button_Red_Pressed.png";

constexpr float kPanelScale = 3.f;
constexpr float kButtonScale = 2.f;
constexpr float kPanelSize = 192.f * kPanelScale;
constexpr float kButtonSize = 64.f * kButtonScale;
}  // namespace

PauseScreen::PauseScreen(EventDispatcher& dispatcher, Window& window)
    : m_eventDispatcher(dispatcher),
      m_window(window),
      m_panelTexture(AssetManager::instance().getTexture(kPanelPath)),
      m_resumeTexture(AssetManager::instance().getTexture(kResumePath)),
      m_resumePressedTexture(AssetManager::instance().getTexture(kResumePressedPath)),
      m_quitTexture(AssetManager::instance().getTexture(kQuitPath)),
      m_quitPressedTexture(AssetManager::instance().getTexture(kQuitPressedPath)),
      m_panel(m_panelTexture),
      m_resumeButton(m_resumeTexture),
      m_quitButton(m_quitTexture),
      m_titleText(m_font),
      m_resumeText(m_font),
      m_quitText(m_font) {
  if (!m_font.openFromFile(Config::Fonts::ARIAL)) {
    std::cerr << "Failed to load font for PauseScreen" << std::endl;
  }

  m_panel.setScale({kPanelScale, kPanelScale});
  m_resumeButton.setScale({kButtonScale, kButtonScale});
  m_quitButton.setScale({kButtonScale, kButtonScale});

  m_titleText.setString("PAUSED");
  m_titleText.setCharacterSize(64);
  m_titleText.setFillColor(sf::Color::White);
  m_titleText.setOutlineColor(sf::Color::Black);
  m_titleText.setOutlineThickness(3.f);

  m_resumeText.setString("Resume");
  m_resumeText.setCharacterSize(24);
  m_resumeText.setFillColor(sf::Color::White);
  m_resumeText.setOutlineColor(sf::Color::Black);
  m_resumeText.setOutlineThickness(2.f);

  m_quitText.setString("Quit");
  m_quitText.setCharacterSize(24);
  m_quitText.setFillColor(sf::Color::White);
  m_quitText.setOutlineColor(sf::Color::Black);
  m_quitText.setOutlineThickness(2.f);

  m_dimmer.setFillColor(sf::Color(0, 0, 0, 150));

  layout();

  m_eventDispatcher.subscribe<DrawEvent>(
      this, [this](DrawEvent& event) { this->onDraw(event); }, RenderLayer::UI);
  m_eventDispatcher.subscribe<KeyPressedEvent>(this,
                                               [this](KeyPressedEvent& event) { this->onKeyPressed(event); });
  m_eventDispatcher.subscribe<MouseLeftClickEvent>(
      this, [this](MouseLeftClickEvent& event) { this->onLeftClick(event); });
}

PauseScreen::~PauseScreen() {
  m_eventDispatcher.unsubscribe(this);
}

void PauseScreen::layout() {
  sf::Vector2u winSize = m_window.getSize();
  sf::Vector2f center(winSize.x / 2.f, winSize.y / 2.f);

  m_dimmer.setSize({static_cast<float>(winSize.x), static_cast<float>(winSize.y)});
  m_dimmer.setPosition({0.f, 0.f});

  m_panel.setPosition({center.x - kPanelSize / 2.f, center.y - kPanelSize / 2.f});

  m_resumeButton.setPosition({center.x - kButtonSize / 2.f, center.y - kButtonSize - 10.f});
  m_quitButton.setPosition({center.x - kButtonSize / 2.f, center.y + 30.f});

  sf::FloatRect titleBounds = m_titleText.getLocalBounds();
  m_titleText.setPosition({center.x - titleBounds.size.x / 2.f - titleBounds.position.x,
                           center.y - kPanelSize / 2.f + 90.f - titleBounds.position.y});

  sf::FloatRect resumeBounds = m_resumeText.getLocalBounds();
  m_resumeText.setPosition({center.x - resumeBounds.size.x / 2.f - resumeBounds.position.x,
                            m_resumeButton.getPosition().y + kButtonSize / 2.f - resumeBounds.size.y / 2.f -
                                resumeBounds.position.y - 4.f});

  sf::FloatRect quitBounds = m_quitText.getLocalBounds();
  m_quitText.setPosition({center.x - quitBounds.size.x / 2.f - quitBounds.position.x,
                          m_quitButton.getPosition().y + kButtonSize / 2.f - quitBounds.size.y / 2.f -
                              quitBounds.position.y - 4.f});
}

void PauseScreen::onKeyPressed(KeyPressedEvent& event) {
  if (event.getKeyboardEvent() == sf::Keyboard::Key::Escape) {
    m_paused = !m_paused;
    m_resumeHeld = false;
    m_quitHeld = false;
    m_resumeButton.setTexture(m_resumeTexture);
    m_quitButton.setTexture(m_quitTexture);
  }
}

void PauseScreen::onLeftClick(MouseLeftClickEvent& event) {
  if (!m_paused) return;

  sf::Vector2i px = event.getPixelPosition();
  sf::Vector2f click(static_cast<float>(px.x), static_cast<float>(px.y));

  if (m_resumeButton.getGlobalBounds().contains(click)) {
    m_paused = false;
    return;
  }

  if (m_quitButton.getGlobalBounds().contains(click)) {
    m_window.close();
    return;
  }
}

void PauseScreen::onDraw(DrawEvent& event) {
  if (!m_paused) return;

  sf::RenderWindow& window = event.getWindow();
  sf::View previousView = window.getView();
  window.setView(window.getDefaultView());

  window.draw(m_dimmer);
  window.draw(m_panel);
  window.draw(m_titleText);
  window.draw(m_resumeButton);
  window.draw(m_quitButton);
  window.draw(m_resumeText);
  window.draw(m_quitText);

  window.setView(previousView);
}
