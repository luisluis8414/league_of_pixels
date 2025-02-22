#include "Entity.h"

Entity::Entity(EventDispatcher& dispatcher,
               int frameWidth,
               int frameHeight,
               sf::Vector2f position,
               float frameTime,
               float speed,
               float maxHealth,
               float physicalDmg,
               EntityType type,
               std::string texturePath,
               RenderLayer renderLayer)
    : m_eventDispatcher(dispatcher),
      m_frameWidth(frameWidth),
      m_frameHeight(frameHeight),
      m_elapsedTime(0.f),
      m_startFrame(0),
      m_endFrame(5),
      m_currentFrame(0),
      m_frameTime(frameTime),
      m_speed(speed),
      m_maxHealth(maxHealth),
      m_currentHealth(maxHealth),
      m_physicalDmg(physicalDmg),
      m_type(type),
      m_destination(position),
      m_target(),
      m_sprite(m_texture) {
  m_texture.setSmooth(false);
  m_sprite.setTexture(m_texture);

  m_frameRect = sf::IntRect({0, 0}, {m_frameWidth, m_frameHeight});
  m_sprite.setTextureRect(m_frameRect);

  m_sprite.setPosition({position.x, position.y});

  m_sprite.setOrigin({m_sprite.getGlobalBounds().size.x / 2.f, m_sprite.getGlobalBounds().size.y / 2.f});

  m_eventDispatcher.subscribe<TickEvent>(this, [this](TickEvent& event) { this->update(event.getDeltaTime()); });

  m_eventDispatcher.subscribe<DrawEvent>(this, [this](DrawEvent& event) { onDraw(event); }, renderLayer);
}

Entity::~Entity() {
  m_eventDispatcher.unsubscribe(this);
}

EntityType Entity::getType() const {
  return m_type;
}

void Entity::update(float deltaTime) {
  if (isTargetInRange(getTarget())) {
    onTargetInRange();
  }
  onUpdate(deltaTime);
}

const sf::FloatRect& Entity::getHitbox() const {
  return m_hitbox;
}

const sf::CircleShape& Entity::getRange() {
  return m_range;
};

const sf::Vector2f Entity::getCenter() {
  sf::Vector2f currentPosition = m_sprite.getPosition();

  sf::FloatRect bounds = m_sprite.getGlobalBounds();

  return sf::Vector2f(currentPosition.x + bounds.size.x / 2.f, currentPosition.y + bounds.size.y / 2.f);
}

const sf::Vector2f Entity::getPosition() {
  return m_sprite.getPosition();
}

void Entity::takeDmg(float dmg) {
  m_currentHealth -= dmg;
}

void Entity::move(float deltaTime) {
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

    {
      sf::FloatRect newXHitbox = m_hitbox;
      newXHitbox.position.x += deltaX;

      sf::FloatRect newYHitbox = m_hitbox;
      newYHitbox.position.y += deltaY;

      bool canMoveX = Map::isTileWalkable(newXHitbox);
      bool canMoveY = Map::isTileWalkable(newYHitbox);

      if (canMoveX) {
        m_sprite.move({deltaX, 0.f});
      } else {
        m_destination.x = m_sprite.getPosition().x;
      }

      if (canMoveY) {
        m_sprite.move({0.f, deltaY});
      } else {
        m_destination.y = m_sprite.getPosition().y;
      }
    }
    setWalking();
  } else {
    setIdle();
  }
}

void Entity::setPosition(sf::Vector2f position) {
  m_sprite.setPosition({position.x, position.y});
};

void Entity::onDmgFrame() {
  if (hasTarget()) {
    std::shared_ptr<Entity> target = getTarget();
    if (target) {
      target->takeDmg(m_physicalDmg);
    }
  }
}

void Entity::setDestination(sf::Vector2f position) {
  sf::Vector2f direction = position - m_sprite.getPosition();

  constexpr float flipThreshold = 1.f;

  if (direction.x < -flipThreshold && m_sprite.getScale().x != -1.f) {
    m_sprite.setScale({-1.f, 1.f});
  } else if (direction.x > flipThreshold && m_sprite.getScale().x != 1.f) {
    m_sprite.setScale({1.f, 1.f});
  }

  m_destination = position;
}

void Entity::updateAnimation(float deltaTime) {
  m_elapsedTime += deltaTime;

  if (m_elapsedTime >= m_frameTime) {
    m_elapsedTime = 0.f;

    m_currentFrame++;
    if (m_currentFrame > m_endFrame) {
      onAnimationEnd();
    }

    if (m_currentFrame == m_dmgFrame) {
      onDmgFrame();
    }

    int column = m_currentFrame % (m_texture.getSize().x / m_frameWidth);
    int row = m_currentFrame / (m_texture.getSize().x / m_frameWidth);
    m_frameRect = sf::IntRect({column * m_frameWidth, row * m_frameHeight}, {m_frameWidth, m_frameHeight});
    m_sprite.setTextureRect(m_frameRect);
  }
}

void Entity::setTarget(std::shared_ptr<Entity> target) {
  m_target = target;
}

void Entity::clearTarget() {
  m_target.reset();

  if (!m_baseTarget.expired()) {
    m_target = m_baseTarget;
  }
}

std::shared_ptr<Entity> Entity::getTarget() const {
  return m_target.lock();
}

bool Entity::hasTarget() const {
  return !m_target.expired();
}

bool Entity::isTargetInRange(std::shared_ptr<Entity> target) {
  if (!target) return false;

  const sf::Rect targetHitbox = target->getHitbox();
  const sf::Rect playerHitbox = getHitbox();

  return Utils::aabbCollision(playerHitbox, targetHitbox);
}
