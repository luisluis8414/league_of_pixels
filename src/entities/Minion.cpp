#include "Minion.h"

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
  if (!m_texture.loadFromFile(texturePath)) {
    std::cerr << "Failed to load sprite sheet: " << texturePath << std::endl;
  }

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

// void Minion::attackEntity(std::shared_ptr<Entity> entity) {
//   if (!hasTarget()) {
//     m_target = entity;

//     if (std::shared_ptr<Entity> target = m_target.lock()) {
//       if (target->getPosition().x < getPosition().x) {
//         m_sprite.setScale({-1.f, 1.f});
//       } else {
//         m_sprite.setScale({1.f, 1.});
//       }
//     }
//   }
// }

// bool Minion::hasTarget() {
//   if (std::shared_ptr<Entity> target = m_target.lock()) {
//     return !m_target.expired() &&
//            Utils::isRectInCircle(target->getHitbox(), getRange());
//   } else {
//     return false;
//   }
// }

void Minion::onAnimationEnd() {
  if (m_state != MinionAnimationState::WALKING) {
    setAnimation(MinionAnimationState::WALKING);
  } else {
    m_currentFrame = m_startFrame;
  }
}

void Minion::onDraw(DrawEvent& event) {
  sf::RenderWindow& window = event.getWindow();
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

    if (isHitting()) {
      sf::RectangleShape attackHitboxShape;
      attackHitboxShape.setPosition({m_attackHitbox.position.x, m_attackHitbox.position.y});
      attackHitboxShape.setSize(sf::Vector2f(m_attackHitbox.size.x, m_attackHitbox.size.y));
      attackHitboxShape.setFillColor(sf::Color::Transparent);
      attackHitboxShape.setOutlineColor(sf::Color::Green);
      attackHitboxShape.setOutlineThickness(1.f);
      window.draw(attackHitboxShape);
    }
  }
}

void Minion::setAnimation(MinionAnimationState animationState) {
  if (m_state != animationState) {
    const AnimationConfig& config = m_animationConfigs.at(animationState);
    m_startFrame = config.startFrame;
    m_endFrame = config.endFrame;
    m_frameTime = config.frameTime;
    m_dmgFrame = config.dmgFrame;
    m_currentFrame = m_startFrame;
    m_state = animationState;
  }
}

void Minion::onUpdate(const float deltaTime) {
  updateHealthBar();
  updateHitbox();
  updateAnimation(deltaTime);

  if (hasTarget()) {
    setDestination(getTarget()->getCenter());
  }

  if (isHitting()) return;

  move(deltaTime);
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

  if (isHitting() && m_attackHitboxConfigs.count(m_state) > 0) {
    const auto& config = m_attackHitboxConfigs.at(m_state);

    if (m_currentFrame >= (m_endFrame - 2)) {
      float attackHitboxWidth = spriteBounds.size.x * config.widthFactor;
      float attackHitboxHeight = spriteBounds.size.y * config.heightFactor;

      float attackHitboxLeft = spriteBounds.position.x + spriteBounds.size.x * config.offsetXFactor;
      float attackHitboxTop = spriteBounds.position.y + spriteBounds.size.y * config.offsetYFactor;

      float direction = (m_sprite.getScale().x < 0.f) ? -1.f : 1.f;
      if (direction < 0.f) {
        float centerX = spriteBounds.position.x + spriteBounds.size.x * 0.5f;
        float offsetFromCenter = attackHitboxLeft - centerX;
        attackHitboxLeft = centerX - offsetFromCenter - attackHitboxWidth;
      }

      m_attackHitbox = sf::FloatRect({attackHitboxLeft, attackHitboxTop}, {attackHitboxWidth, attackHitboxHeight});
    }

  } else {
    // reset when not attacking
    m_attackHitbox = sf::FloatRect({0.f, 0.f}, {0.f, 0.f});
  }
}

bool Minion::isHitting() const {
  return (m_state != MinionAnimationState::WALKING);
}

void Minion::setWalking() {
  if (m_state != MinionAnimationState::WALKING) {
    setAnimation(MinionAnimationState::WALKING);
  }
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

void Minion::onDmgFrame() {
  if (hasTarget()) {
    getTarget()->takeDmg(m_physicalDmg);
  }
}
