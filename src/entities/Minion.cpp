#include "Minion.h"

#include "../components/Map.h"
#include "../core/Config.h"
#include "../core/Utils.h"

Minion::Minion(EventDispatcher& dispatcher,
               const std::string& texturePath,
               sf::Vector2f position,
               sf::Vector2f destination,
               sf::Color healthbarColor)
    : Entity(dispatcher,
             192,
             192,
             position,
             0.1f,
             100.f,
             100.f,
             5.f,
             EntityType::Minion,
             texturePath,
             RenderLayer::ENTITIES),
      m_state(MinionAnimationState::WALKING) {
  m_range.setRadius(120.f);
  m_range.setOrigin({m_range.getRadius(), m_range.getRadius()});
  m_range.setPosition(getCenter());
  m_range.setFillColor(sf::Color(0, 0, 0, 0));
  m_range.setOutlineColor(sf::Color::Red);
  m_range.setOutlineThickness(1.5f);

  m_healthBarBackground.setSize(sf::Vector2f(100.f, 10.f));
  m_healthBarBackground.setFillColor(sf::Color::Red);
  m_healthBarBackground.setPosition({10.f, 10.f});

  m_healthBarForeground.setSize(sf::Vector2f(100.f, 10.f));
  m_healthBarForeground.setFillColor(sf::Color::Green);
  m_healthBarForeground.setPosition({10.f, 10.f});

  setDestination(destination);

  m_healthBarBackground.setSize({50.f, 5.f});
  m_healthBarBackground.setFillColor(sf::Color::Black);

  m_healthBarForeground.setSize({50.f, 5.f});
  m_healthBarForeground.setFillColor(healthbarColor);
}

void Minion::onAnimationEnd() {
  if (m_state != MinionAnimationState::WALKING) {
    setAnimation(MinionAnimationState::WALKING);
  } else {
    m_currentFrame = m_startFrame;
  }
}

void Minion::onDraw(DrawEvent& event) {
  sf::RenderWindow& window = event.getWindow();
  sf::FloatRect bounds = m_sprite.getGlobalBounds();
  sf::CircleShape shadow(bounds.size.x * 0.11f);
  shadow.setScale({1.45f, 0.3f});
  shadow.setOrigin({shadow.getRadius(), shadow.getRadius()});
  shadow.setPosition({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y * 0.68f});
  shadow.setFillColor(sf::Color(0, 0, 0, 55));
  window.draw(shadow);

  drawHoverOutline(window);
  window.draw(m_sprite);

  window.draw(m_healthBarBackground);
  window.draw(m_healthBarForeground);

  if (Config::Settings::showHitboxes) {
    sf::RectangleShape hitboxShape;
    hitboxShape.setPosition({m_hitbox.position.x, m_hitbox.position.y});
    hitboxShape.setSize(sf::Vector2f(m_hitbox.size.x, m_hitbox.size.y));
    hitboxShape.setFillColor(sf::Color::Transparent);
    hitboxShape.setOutlineColor(sf::Color::Red);
    hitboxShape.setOutlineThickness(1.f);

    window.draw(m_range);

    window.draw(hitboxShape);
  }
}

void Minion::setAnimation(MinionAnimationState animationState) {
  if (m_state == animationState) return;
  const AnimationConfig& config = m_animationConfigs.at(animationState);
  m_startFrame = config.startFrame;
  m_endFrame = config.endFrame;
  m_frameTime = config.frameTime;
  m_dmgFrame = config.dmgFrame;
  m_currentFrame = m_startFrame;
  m_state = animationState;
}

void Minion::onUpdate(const float deltaTime) {
  updateHealthBar();
  updateHitbox();
  updateAnimation(deltaTime);

  if (hasTarget()) {
    if (getTarget()->getType() == EntityType::Building) {
      setDestination(getTarget()->getCenter() - sf::Vector2f({0.f, -20.f}));
    } else {
      setDestination(getTarget()->getPosition());
    }
  }

  if (isHitting()) return;

  moveAroundBlockers(deltaTime);
}

void Minion::updateHealthBar() {
  if (m_currentHealth <= 0) {
    DestroyEntityEvent destroyEvent(this);
    m_eventDispatcher.emit(destroyEvent);
    return;
  }
  float healthPercentage = m_currentHealth / m_maxHealth;
  m_healthBarForeground.setSize({healthPercentage * 50.f, 5.f});

  sf::FloatRect bounds = m_sprite.getGlobalBounds();
  float healthBarX = bounds.position.x + (bounds.size.x / 2.f) - (m_healthBarBackground.getSize().x / 2.f);
  float healthBarY = bounds.position.y - m_healthBarBackground.getSize().y + 60.f;
  m_healthBarBackground.setPosition({healthBarX, healthBarY});
  m_healthBarForeground.setPosition({healthBarX, healthBarY});
}

void Minion::updateHitbox() {
  m_range.setPosition(m_sprite.getPosition());

  sf::FloatRect spriteBounds = m_sprite.getGlobalBounds();

  float hitboxWidth = spriteBounds.size.x * 0.2f;
  float hitboxHeight = spriteBounds.size.y * 0.3f;

  float hitboxLeft = spriteBounds.position.x + (spriteBounds.size.x - hitboxWidth) / 2.f;
  float hitboxTop = spriteBounds.position.y + (spriteBounds.size.y - hitboxHeight) / 2.f;

  m_hitbox = sf::FloatRect({hitboxLeft, hitboxTop}, {hitboxWidth, hitboxHeight});
}

bool Minion::isHitting() const {
  return (m_state != MinionAnimationState::WALKING);
}

void Minion::setMovementBlockers(const std::vector<sf::FloatRect>& blockers) {
  m_movementBlockers = blockers;
}

bool Minion::canOccupy(const sf::FloatRect& hitbox) const {
  if (!Map::isTileWalkable(hitbox)) return false;

  for (const sf::FloatRect& blocker : m_movementBlockers) {
    if (Utils::aabbCollision(hitbox, blocker)) return false;
  }

  return true;
}

float Minion::getBlockerOverlap(const sf::FloatRect& hitbox) const {
  float overlap = 0.f;

  for (const sf::FloatRect& blocker : m_movementBlockers) {
    const float left = std::max(hitbox.position.x, blocker.position.x);
    const float right = std::min(hitbox.position.x + hitbox.size.x, blocker.position.x + blocker.size.x);
    const float top = std::max(hitbox.position.y, blocker.position.y);
    const float bottom = std::min(hitbox.position.y + hitbox.size.y, blocker.position.y + blocker.size.y);

    if (right > left && bottom > top) {
      overlap += (right - left) * (bottom - top);
    }
  }

  return overlap;
}

void Minion::moveAroundBlockers(float deltaTime) {
  sf::Vector2f direction = m_destination - m_sprite.getPosition();
  float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
  constexpr float epsilon = 2.f;

  if (distance <= epsilon) {
    setIdle();
    return;
  }

  direction /= distance;

  constexpr float minDirectionThreshold = 0.01f;
  if (std::abs(direction.x) < minDirectionThreshold) direction.x = 0.0f;
  if (std::abs(direction.y) < minDirectionThreshold) direction.y = 0.0f;

  sf::Vector2f step = direction * m_speed * deltaTime;
  sf::FloatRect nextHitbox = m_hitbox;
  nextHitbox.position += step;

  if (canOccupy(nextHitbox)) {
    m_sprite.move(step);
    setWalking();
    return;
  }

  const float sideStepAmount = std::max(std::abs(step.y), m_speed * deltaTime);
  const float preferredDirection = getCenter().y >= m_destination.y ? 1.f : -1.f;
  const float sideDirections[2] = {preferredDirection, -preferredDirection};
  const float currentOverlap = getBlockerOverlap(m_hitbox);

  for (float sideDirection : sideDirections) {
    sf::Vector2f sideStep = {0.f, sideDirection * sideStepAmount};
    sf::FloatRect sideHitbox = m_hitbox;
    sideHitbox.position += sideStep;

    if (canOccupy(sideHitbox) ||
        (Map::isTileWalkable(sideHitbox) && currentOverlap > 0.f && getBlockerOverlap(sideHitbox) < currentOverlap)) {
      m_sprite.move(sideStep);
      setWalking();
      return;
    }
  }

  setIdle();
}

void Minion::setWalking() {
  setAnimation(MinionAnimationState::WALKING);
}

void Minion::setIdle() {
  setWalking();
}

void Minion::onTargetInRange() {
  setAnimation(MinionAnimationState::HITTING);

  if (getTarget()->getPosition().x < getPosition().x) {
    m_sprite.setScale({-1.f, 1.f});
  } else {
    m_sprite.setScale({1.f, 1.f});
  }
}
