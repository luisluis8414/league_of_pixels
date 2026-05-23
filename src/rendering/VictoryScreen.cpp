#include "VictoryScreen.h"

#include <iostream>

#include "../core/AssetManager.h"
#include "../core/Config.h"

namespace {
const std::string kPanelPath = "resources/tiny_swords/UI/Banners/Banner_Vertical.png";
const std::string kExitPath = "resources/tiny_swords/UI/Buttons/Button_Red.png";

constexpr float kPanelScale = 3.f;
constexpr float kButtonScale = 2.f;
constexpr float kPanelSize = 192.f * kPanelScale;
constexpr float kButtonSize = 64.f * kButtonScale;
}  // namespace

VictoryScreen::VictoryScreen(EventDispatcher& dispatcher, Window& window)
    : m_eventDispatcher(dispatcher),
      m_window(window),
      m_panelTexture(AssetManager::instance().getTexture(kPanelPath)),
      m_exitTexture(AssetManager::instance().getTexture(kExitPath)),
      m_panel(m_panelTexture),
      m_exitButton(m_exitTexture),
      m_titleText(m_font),
      m_exitText(m_font) {
  if (!m_font.openFromFile(Config::Fonts::ARIAL)) {
    std::cerr << "Failed to load font for VictoryScreen" << std::endl;
  }

  m_panel.setScale({kPanelScale, kPanelScale});
  m_exitButton.setScale({kButtonScale, kButtonScale});

  m_titleText.setString("VICTORY");
  m_titleText.setCharacterSize(72);
  m_titleText.setFillColor(sf::Color(255, 215, 0));
  m_titleText.setOutlineColor(sf::Color::Black);
  m_titleText.setOutlineThickness(3.f);

  m_exitText.setString("Exit");
  m_exitText.setCharacterSize(24);
  m_exitText.setFillColor(sf::Color::White);
  m_exitText.setOutlineColor(sf::Color::Black);
  m_exitText.setOutlineThickness(2.f);

  m_dimmer.setFillColor(sf::Color(0, 0, 0, 180));

  layout();

  m_eventDispatcher.subscribe<DrawEvent>(
      this, [this](DrawEvent& event) { this->onDraw(event); }, RenderLayer::UI);
  m_eventDispatcher.subscribe<MouseLeftClickEvent>(
      this, [this](MouseLeftClickEvent& event) { this->onLeftClick(event); });
  m_eventDispatcher.subscribe<GameOverEvent>(this,
                                             [this](GameOverEvent& event) { this->onGameOver(event); });
}

VictoryScreen::~VictoryScreen() {
  m_eventDispatcher.unsubscribe(this);
}

void VictoryScreen::layout() {
  sf::Vector2u winSize = m_window.getSize();
  sf::Vector2f center(winSize.x / 2.f, winSize.y / 2.f);

  m_dimmer.setSize({static_cast<float>(winSize.x), static_cast<float>(winSize.y)});
  m_dimmer.setPosition({0.f, 0.f});

  m_panel.setPosition({center.x - kPanelSize / 2.f, center.y - kPanelSize / 2.f});

  m_exitButton.setPosition({center.x - kButtonSize / 2.f, center.y + 60.f - kButtonSize / 2.f});

  sf::FloatRect titleBounds = m_titleText.getLocalBounds();
  m_titleText.setPosition({center.x - titleBounds.size.x / 2.f - titleBounds.position.x,
                           center.y - kPanelSize / 2.f + 100.f - titleBounds.position.y});

  sf::FloatRect exitBounds = m_exitText.getLocalBounds();
  sf::Vector2f exitPos = m_exitButton.getPosition();
  m_exitText.setPosition({exitPos.x + kButtonSize / 2.f - exitBounds.size.x / 2.f - exitBounds.position.x,
                          exitPos.y + kButtonSize / 2.f - exitBounds.size.y / 2.f - exitBounds.position.y - 4.f});
}

void VictoryScreen::onGameOver(GameOverEvent& event) {
  m_visible = true;
}

void VictoryScreen::onLeftClick(MouseLeftClickEvent& event) {
  if (!m_visible) return;

  sf::Vector2i px = event.getPixelPosition();
  sf::Vector2f click(static_cast<float>(px.x), static_cast<float>(px.y));

  if (m_exitButton.getGlobalBounds().contains(click)) {
    m_window.close();
  }
}

void VictoryScreen::onDraw(DrawEvent& event) {
  if (!m_visible) return;

  sf::RenderWindow& window = event.getWindow();
  sf::View previousView = window.getView();
  window.setView(window.getDefaultView());

  window.draw(m_dimmer);
  window.draw(m_panel);
  window.draw(m_titleText);
  window.draw(m_exitButton);
  window.draw(m_exitText);

  window.setView(previousView);
}
