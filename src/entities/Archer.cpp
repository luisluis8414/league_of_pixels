#include "Archer.h"

#include <memory>

#include "../core/Config.h"
#include "../projectiles/Projectile.h"

Archer::Archer(EventDispatcher& dispatcher,
               sf::Vector2f position,
               const std::string& texturePath,
               std::weak_ptr<Entity>& target)
    : Entity(dispatcher, 192, 192, position, 0.1f, 200.f, 100.f, 10.f, EntityType::Archer, texturePath),
      m_state(ArcherAnimationState::IDLE),
      m_target(target) {
  if (!m_texture.loadFromFile(texturePath)) {
    std::cerr << "Failed to load sprite sheet: " << texturePath << std::endl;
  }

  m_texture.setSmooth(false);
  m_sprite.setTexture(m_texture);

  m_frameRect = sf::IntRect({0, 0}, {m_frameWidth, m_frameHeight});
  m_sprite.setTextureRect(m_frameRect);

  m_sprite.setPosition({position.x, position.y});

  m_sprite.setOrigin({m_sprite.getGlobalBounds().size.x / 2.f, m_sprite.getGlobalBounds().size.y / 2.f});

  m_healthBarBackground.setSize(sf::Vector2f(100.f, 10.f));
  m_healthBarBackground.setFillColor(sf::Color::Red);
  m_healthBarBackground.setPosition({10.f, 10.f});

  m_healthBarForeground.setSize(sf::Vector2f(100.f, 10.f));
  m_healthBarForeground.setFillColor(sf::Color::Green);
  m_healthBarForeground.setPosition({10.f, 10.f});
  m_eventDispatcher.subscribe<DrawEvent>(
      this, [this](DrawEvent& event) { this->onDraw(event); }, RenderLayer::ENTITIES);

  m_eventDispatcher.subscribe<TickEvent>(this, [this](TickEvent& event) { this->onUpdate(event.getDeltaTime()); });
}

void Archer::onUpdate(float deltaTime) {
  updateAnimation(deltaTime);
};

void Archer::onDraw(DrawEvent& event) {
  sf::RenderWindow& window = event.getWindow();
  window.draw(m_sprite);
};

void Archer::onAnimationEnd() {
  if (m_state != ArcherAnimationState::IDLE) {
    setAnimation(ArcherAnimationState::IDLE);
  } else {
    m_currentFrame = m_startFrame;
  }
}

void Archer::setAnimation(ArcherAnimationState animationState) {
  if (animationState == m_state) return;
  if (m_animationConfigs.count(animationState) > 0) {
    const AnimationConfig& config = m_animationConfigs.at(animationState);
    m_startFrame = config.startFrame;
    m_endFrame = config.endFrame;
    m_frameTime = config.frameTime;
    m_dmgFrame = 46;
    m_currentFrame = m_startFrame;
  }
  m_state = animationState;
}

void Archer::attackEntity(std::weak_ptr<Entity>& target) {
  setAnimation(ArcherAnimationState::SHOOTING);

  if (std::shared_ptr<Entity> target = m_target.lock()) {
    if (target->getPosition().x < getPosition().x) {
      m_sprite.setScale({-1.f, 1.f});
    } else {
      m_sprite.setScale({1.f, 1.});
    }
  }

  m_target = target;
}

void Archer::onDmgFrame() {
  if (std::shared_ptr<Entity> target = m_target.lock()) {
    std::shared_ptr<Projectile> projectile = std::make_shared<Projectile>(
        m_eventDispatcher, Config::Textures::Projectiles::ARROW, getPosition(), target, 500.0f);

    RegisterProjectileEvent event(projectile);
    m_eventDispatcher.emit(event);
    m_target.reset();
  }
}

void Archer::setWalking() {
  setIdle();
}

void Archer::setIdle() {
  if (m_state != ArcherAnimationState::IDLE) {
    setAnimation(ArcherAnimationState::IDLE);
  }
}