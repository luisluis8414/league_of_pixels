#include "PauseScreen.h"

#include <iostream>

#include "../core/AssetManager.h"
#include "../core/Config.h"

namespace {
const std::string kPanelPath = "resources/tiny_swords/UI/Banners/Banner_Vertical.png";
const std::string kQuitPath = "resources/tiny_swords/UI/Buttons/Button_Red.png";
const std::string kToggleOffPath = "resources/tiny_swords/UI/Buttons/Button_Blue.png";
const std::string kToggleOnPath = "resources/tiny_swords/UI/Buttons/Button_Blue_Pressed.png";

constexpr float kPanelScale = 3.f;
constexpr float kButtonScale = 2.f;
constexpr float kPanelSize = 192.f * kPanelScale;
constexpr float kButtonSize = 64.f * kButtonScale;
}  // namespace

PauseScreen::PauseScreen(EventDispatcher& dispatcher, Window& window)
    : m_eventDispatcher(dispatcher),
      m_window(window),
      m_panelTexture(AssetManager::instance().getTexture(kPanelPath)),
      m_quitTexture(AssetManager::instance().getTexture(kQuitPath)),
      m_toggleOffTexture(AssetManager::instance().getTexture(kToggleOffPath)),
      m_toggleOnTexture(AssetManager::instance().getTexture(kToggleOnPath)),
      m_panel(m_panelTexture),
      m_hitboxButton(m_toggleOffTexture),
      m_quitButton(m_quitTexture),
      m_titleText(m_font),
      m_hitboxText(m_font),
      m_quitText(m_font) {
  if (!m_font.openFromFile(Config::Fonts::ARIAL)) {
    std::cerr << "Failed to load font for PauseScreen" << std::endl;
  }

  m_panel.setScale({kPanelScale, kPanelScale});
  m_hitboxButton.setScale({kButtonScale, kButtonScale});
  m_quitButton.setScale({kButtonScale, kButtonScale});

  m_titleText.setString("MENU");
  m_titleText.setCharacterSize(64);
  m_titleText.setFillColor(sf::Color::White);
  m_titleText.setOutlineColor(sf::Color::Black);
  m_titleText.setOutlineThickness(3.f);

  auto initButtonText = [](sf::Text& text, const std::string& str) {
    text.setString(str);
    text.setCharacterSize(22);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2.f);
  };
  initButtonText(m_hitboxText, "Hitboxes");
  initButtonText(m_quitText, "Quit");

  m_dimmer.setFillColor(sf::Color(0, 0, 0, 150));

  refreshHitboxButton();
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

void PauseScreen::refreshHitboxButton() {
  m_hitboxButton.setTexture(Config::Settings::showHitboxes ? m_toggleOnTexture : m_toggleOffTexture);
}

void PauseScreen::layout() {
  sf::Vector2u winSize = m_window.getSize();
  sf::Vector2f center(winSize.x / 2.f, winSize.y / 2.f);

  m_dimmer.setSize({static_cast<float>(winSize.x), static_cast<float>(winSize.y)});
  m_dimmer.setPosition({0.f, 0.f});

  m_panel.setPosition({center.x - kPanelSize / 2.f, center.y - kPanelSize / 2.f});

  const float buttonCenterX = center.x - kButtonSize / 2.f;
  m_hitboxButton.setPosition({buttonCenterX, center.y - 20.f - kButtonSize / 2.f});
  m_quitButton.setPosition({buttonCenterX, center.y + 120.f - kButtonSize / 2.f});

  sf::FloatRect titleBounds = m_titleText.getLocalBounds();
  m_titleText.setPosition({center.x - titleBounds.size.x / 2.f - titleBounds.position.x,
                           center.y - kPanelSize / 2.f + 90.f - titleBounds.position.y});

  auto centerOnButton = [&](sf::Text& text, const sf::Sprite& button) {
    sf::FloatRect bounds = text.getLocalBounds();
    sf::Vector2f pos = button.getPosition();
    text.setPosition({pos.x + kButtonSize / 2.f - bounds.size.x / 2.f - bounds.position.x,
                      pos.y + kButtonSize / 2.f - bounds.size.y / 2.f - bounds.position.y - 4.f});
  };
  centerOnButton(m_hitboxText, m_hitboxButton);
  centerOnButton(m_quitText, m_quitButton);
}

void PauseScreen::onKeyPressed(KeyPressedEvent& event) {
  if (event.getKeyboardEvent() == sf::Keyboard::Key::Escape) {
    m_paused = !m_paused;
  }
}

void PauseScreen::onLeftClick(MouseLeftClickEvent& event) {
  if (!m_paused) return;

  sf::Vector2i px = event.getPixelPosition();
  sf::Vector2f click(static_cast<float>(px.x), static_cast<float>(px.y));

  if (m_hitboxButton.getGlobalBounds().contains(click)) {
    Config::Settings::showHitboxes = !Config::Settings::showHitboxes;
    refreshHitboxButton();
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
  window.draw(m_hitboxButton);
  window.draw(m_quitButton);
  window.draw(m_hitboxText);
  window.draw(m_quitText);

  window.setView(previousView);
}
