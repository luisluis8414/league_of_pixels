#include "Minion.h"

#include "../core/Config.h"

Minion::Minion(EventDispatcher& dispatcher,
               const std::string& texturePath,
               sf::Vector2f position,
               sf::Vector2f destination)
    : Entity(dispatcher, 192, 192, position, 0.1f, 100.f, 100.f, 5.f, EntityType::Minion, texturePath),
      m_state(MinionAnimationState::Walking) {
  m_dispatcher.subscribe<DrawEvent>(this, [this](DrawEvent& event) { onDraw(event); }, RenderLayer::ENTITIES);

  m_dispatcher.subscribe<TickEvent>(this, [this](TickEvent& event) { onUpdate(event.getDeltaTime()); });

  setDestination(destination);

  m_dispatcher.subscribe<CollisionEvent>(this, [this](CollisionEvent& event) {
    const Entity& entityA = event.getEntityA();
    const Entity& entityB = event.getEntityB();

    if (&entityA == this || &entityB == this) {
      // onCollision();
    }
  });
}

void Minion::onAnimationEnd() {
  if (m_state != MinionAnimationState::Walking) {
    setAnimation(MinionAnimationState::Walking);
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

    sf::FloatRect bounds = m_sprite.getGlobalBounds();

    sf::RectangleShape border;
    border.setPosition({bounds.position.x, bounds.position.y});
    border.setSize({bounds.size.x, bounds.size.y});
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::Red);
    border.setOutlineThickness(1.f);

    window.draw(border);
  }
}

void Minion::setAnimation(MinionAnimationState animationState) {
  if (m_animationConfigs.count(animationState) > 0) {
    const auto& config = m_animationConfigs.at(animationState);
    m_startFrame = config.startFrame;
    m_endFrame = config.endFrame;
    m_frameTime = config.frameTime;
    m_currentFrame = m_startFrame;
  }
  m_state = animationState;
}

void Minion::move(float deltaTime) {
  sf::Vector2f direction = m_destination - m_sprite.getPosition();

  float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

  constexpr float epsilon = 2.f;

  if (distance > epsilon) {
    direction /= distance;

    constexpr float minDirectionThreshold = 0.01f;
    if (std::abs(direction.x) < minDirectionThreshold) direction.x = 0.0f;
    if (std::abs(direction.y) < minDirectionThreshold) direction.y = 0.0f;

    float deltaX = direction.x * m_speed * deltaTime;
    float deltaY = direction.y * m_speed * deltaTime;

    MoveEvent moveEvent(m_sprite, m_hitbox, {deltaX, deltaY}, m_destination);

    m_dispatcher.emit(moveEvent);

    if (m_state != MinionAnimationState::Walking) {
      setAnimation(MinionAnimationState::Walking);
    }
  }
}

void Minion::onUpdate(const float deltaTime) {
  updateHealthBar();
  updateHitbox();
  updateAnimation(deltaTime);

  if (isHitting()) return;

  move(deltaTime);
}

void Minion::updateHealthBar() {
  if (m_currentHealth <= 0) {
    DestroyEntityEvent destroyEvent(this);
    m_dispatcher.emit(destroyEvent);
    return;
  }
  float healthPercentage = m_currentHealth / m_maxHealth;
  m_healthBarForeground.setSize(sf::Vector2f(healthPercentage * 100.f, 10.f));

  sf::FloatRect bounds = m_sprite.getGlobalBounds();
  float healthBarX = bounds.position.x + (bounds.size.x / 2.f) - (m_healthBarBackground.getSize().x / 2.f);
  float healthBarY = bounds.position.y - m_healthBarBackground.getSize().y + 25.f;  // offset for spacing from top
  m_healthBarBackground.setPosition({healthBarX, healthBarY});
  m_healthBarForeground.setPosition({healthBarX, healthBarY});
}

void Minion::updateHitbox() {
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
  return (m_state != MinionAnimationState::Walking);
}
