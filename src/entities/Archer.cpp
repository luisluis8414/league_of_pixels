#include "Archer.h"

#include "../core/Config.h"

Archer::Archer(EventDispatcher& dispatcher, sf::Vector2f position, const std::string& texturePath)
    : Entity(dispatcher, 192, 192, position, 0.1f, 200.f, 100.f, 10.f, EntityType::Archer, texturePath),
      m_state(ArcherAnimationState::IDLE) {
  m_dispatcher.subscribe<DrawEvent>(this, [this](DrawEvent& event) { this->onDraw(event); }, RenderLayer::ENTITIES);

  m_dispatcher.subscribe<TickEvent>(this, [this](TickEvent& event) { this->onUpdate(event.getDeltaTime()); });
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
    m_currentFrame = m_startFrame;
  }
  m_state = animationState;
}

void Archer::attackEntity(const Entity& entity) {
  setAnimation(ArcherAnimationState::SHOOTING);
}