#include "Archer.h"

#include "../core/Config.h"

Archer::Archer(EventDispatcher& dispatcher, sf::Vector2f position, const std::string& texturePath)
    : Entity(dispatcher, 192, 192, position, 0.1f, 200.f, 100.f, 10.f, EntityType::Archer, texturePath),
      m_state(ArcherAnimationState::IDLE) {
  m_dispatcher.subscribe<DrawEvent>(this, [this](DrawEvent& event) { this->onDraw(event); }, RenderLayer::PLAYER);

  m_dispatcher.subscribe<TickEvent>(this, [this](TickEvent& event) { this->onUpdate(event.getDeltaTime()); });
}

void Archer::onUpdate(float deltaTime) {
  updateAnimation(deltaTime);
};

void Archer::onDraw(DrawEvent& event) {
  sf::RenderWindow& window = event.getWindow();
  window.draw(m_sprite);
};

void Archer::updateAnimation(float deltaTime) {
  m_elapsedTime += deltaTime;

  if (m_elapsedTime >= m_frameTime) {
    m_elapsedTime = 0.f;

    m_currentFrame++;
    if (m_currentFrame > m_endFrame) {
      if (m_state != ArcherAnimationState::IDLE) {
        setAnimation(ArcherAnimationState::IDLE);
      } else {
        m_currentFrame = m_startFrame;
      }
    }

    int column = m_currentFrame % (m_texture.getSize().x / m_frameWidth);
    int row = m_currentFrame / (m_texture.getSize().x / m_frameWidth);
    m_frameRect = sf::IntRect({column * m_frameWidth, row * m_frameHeight}, {m_frameWidth, m_frameHeight});
    m_sprite.setTextureRect(m_frameRect);
  }
}

void Archer::setAnimation(ArcherAnimationState animationState) {
  if (animationState == m_state) return;
  if (m_animationConfigs.count(animationState) > 0) {
    const AnimationConfig& config = m_animationConfigs.at(animationState);
    m_startFrame = config.startFrame;
    m_endFrame = config.endFrame;
    m_frameTime = config.frameTime;
    m_currentFrame = m_startFrame;
  }
  m_state = animationState;
}

void Archer::attackEntity(const Entity& entity) {
  setAnimation(ArcherAnimationState::SHOOTING);
}