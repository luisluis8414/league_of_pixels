#include "AbilityBar.h"

#include <cmath>
#include <iostream>

namespace {
constexpr float kSlotSize = 72.f;
constexpr float kSlotGap = 10.f;
constexpr float kBottomMargin = 32.f;
constexpr float kIconPadding = 7.f;
}  // namespace

AbilityBar::AbilityBar(EventDispatcher& dispatcher, const Player& player, const std::string& fontPath)
    : m_eventDispatcher(dispatcher), m_player(player), m_keyText(m_font), m_cooldownText(m_font) {
  if (!m_font.openFromFile(fontPath)) {
    std::cerr << "Failed to load font for AbilityBar: " << fontPath << std::endl;
  }

  m_keyText.setCharacterSize(16);
  m_keyText.setFillColor(sf::Color::White);
  m_keyText.setOutlineColor(sf::Color::Black);
  m_keyText.setOutlineThickness(2.f);

  m_cooldownText.setCharacterSize(26);
  m_cooldownText.setFillColor(sf::Color::White);
  m_cooldownText.setOutlineColor(sf::Color::Black);
  m_cooldownText.setOutlineThickness(2.f);

  m_eventDispatcher.subscribe<DrawEvent>(
      this, [this](DrawEvent& event) { this->onDraw(event); }, RenderLayer::TEXT);
}

AbilityBar::~AbilityBar() {
  m_eventDispatcher.unsubscribe(this);
}

void AbilityBar::onDraw(DrawEvent& event) {
  sf::RenderWindow& window = event.getWindow();
  sf::View previousView = window.getView();
  window.setView(window.getDefaultView());

  const std::array<AbilityCooldownView, 4>& abilities = m_player.getAbilityCooldowns();
  const sf::Vector2u windowSize = window.getSize();
  const float totalWidth = abilities.size() * kSlotSize + (abilities.size() - 1) * kSlotGap;
  const float startX = (static_cast<float>(windowSize.x) - totalWidth) / 2.f;
  const float y = static_cast<float>(windowSize.y) - kSlotSize - kBottomMargin;

  for (std::size_t i = 0; i < abilities.size(); ++i) {
    drawSlot(window, abilities[i], {startX + i * (kSlotSize + kSlotGap), y});
  }

  window.setView(previousView);
}

void AbilityBar::drawSlot(sf::RenderWindow& window, const AbilityCooldownView& ability, sf::Vector2f position) {
  sf::RectangleShape slot({kSlotSize, kSlotSize});
  slot.setPosition(position);
  slot.setFillColor(sf::Color(24, 28, 36, 235));
  slot.setOutlineColor(ability.cooldownRemaining > 0.f ? sf::Color(90, 90, 100) : sf::Color(205, 170, 80));
  slot.setOutlineThickness(2.f);
  window.draw(slot);

  if (ability.texture) {
    sf::Sprite icon(*ability.texture);
    icon.setTextureRect(ability.iconRect);

    const sf::FloatRect iconBounds = icon.getLocalBounds();
    const float iconScale =
        std::min((kSlotSize - kIconPadding * 2.f) / iconBounds.size.x, (kSlotSize - kIconPadding * 2.f) / iconBounds.size.y);
    icon.setScale({iconScale, iconScale});
    icon.setPosition({position.x + (kSlotSize - iconBounds.size.x * iconScale) / 2.f - iconBounds.position.x * iconScale,
                      position.y + (kSlotSize - iconBounds.size.y * iconScale) / 2.f - iconBounds.position.y * iconScale});
    window.draw(icon);
  }

  if (ability.cooldownRemaining > 0.f) {
    const float cooldownPercent = ability.cooldownRemaining / ability.cooldownDuration;
    sf::RectangleShape dim({kSlotSize, kSlotSize});
    dim.setPosition(position);
    dim.setFillColor(sf::Color(0, 0, 0, 125));
    window.draw(dim);

    sf::RectangleShape cooldownWipe({kSlotSize, kSlotSize * cooldownPercent});
    cooldownWipe.setPosition(position);
    cooldownWipe.setFillColor(sf::Color(0, 0, 0, 105));
    window.draw(cooldownWipe);

    m_cooldownText.setString(std::to_string(static_cast<int>(std::ceil(ability.cooldownRemaining))));
    sf::FloatRect cooldownBounds = m_cooldownText.getLocalBounds();
    m_cooldownText.setPosition({position.x + kSlotSize / 2.f - cooldownBounds.size.x / 2.f - cooldownBounds.position.x,
                                position.y + kSlotSize / 2.f - cooldownBounds.size.y / 2.f - cooldownBounds.position.y});
    window.draw(m_cooldownText);
  }

  sf::RectangleShape keyBadge({22.f, 20.f});
  keyBadge.setPosition({position.x + 4.f, position.y + kSlotSize - 24.f});
  keyBadge.setFillColor(sf::Color(0, 0, 0, 185));
  keyBadge.setOutlineColor(sf::Color(255, 255, 255, 80));
  keyBadge.setOutlineThickness(1.f);
  window.draw(keyBadge);

  m_keyText.setString(std::string(1, ability.key));
  sf::FloatRect keyBounds = m_keyText.getLocalBounds();
  m_keyText.setPosition({keyBadge.getPosition().x + keyBadge.getSize().x / 2.f - keyBounds.size.x / 2.f - keyBounds.position.x,
                         keyBadge.getPosition().y + keyBadge.getSize().y / 2.f - keyBounds.size.y / 2.f - keyBounds.position.y - 1.f});
  window.draw(m_keyText);
}
