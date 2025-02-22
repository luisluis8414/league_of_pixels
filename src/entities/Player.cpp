#include "Player.h"

#include "../core/Config.h"
#include "../core/Utils.h"

Player::Player(EventDispatcher& dispatcher,
               sf::Vector2f position,
               const std::string& qTexturePath,
               const std::string& wTexturePath,
               const std::string& eTexturePath,
               const std::string& rTexturePath)
    : Entity(dispatcher,
             192,
             192,
             position,
             0.1f,
             200.f,
             100.f,
             10.f,
             EntityType::Player,
             Config::Textures::Troops::PLAYER,
             RenderLayer::PLAYER),
      m_state(PlayerAnimationState::IDLE) {
  if (!m_texture.loadFromFile(Config::Textures::Troops::PLAYER)) {
    std::cerr << "Failed to load sprite sheet: " << Config::Textures::Troops::PLAYER << std::endl;
  }

  m_healthBarBackground.setSize(sf::Vector2f(100.f, 10.f));
  m_healthBarBackground.setFillColor(sf::Color::Red);
  m_healthBarBackground.setPosition({10.f, 10.f});

  m_healthBarForeground.setSize(sf::Vector2f(100.f, 10.f));
  m_healthBarForeground.setFillColor(sf::Color::Green);
  m_healthBarForeground.setPosition({10.f, 10.f});

  if (!m_qTexture.loadFromFile(qTexturePath)) {
    std::cerr << "Failed to load sprite sheet: " << qTexturePath << std::endl;
  }

  if (!m_wTexture.loadFromFile(wTexturePath)) {
    std::cerr << "Failed to load sprite sheet: " << wTexturePath << std::endl;
  }

  if (!m_eTexture.loadFromFile(eTexturePath)) {
    std::cerr << "Failed to load sprite sheet: " << eTexturePath << std::endl;
  }

  if (!m_rTexture.loadFromFile(rTexturePath)) {
    std::cerr << "Failed to load sprite sheet: " << rTexturePath << std::endl;
  }

  subscribe();
}

void Player::subscribe() {
  m_eventDispatcher.subscribe<DestroyEntityEvent>(this, [this](DestroyEntityEvent& event) {
    std::shared_ptr<Entity> target = getTarget();
    if (target && target.get() == event.getEntity()) {
      clearTarget();
      setDestination(m_sprite.getPosition());
    }
  });

  m_eventDispatcher.subscribe<KeyPressedEvent>(this, [this](KeyPressedEvent& event) {
    if (event.getKeyboardEvent() == sf::Keyboard::Key::Q) {
      castAbility(PlayerAbilities::Q, event.getMousePosition());
    }
    if (event.getKeyboardEvent() == sf::Keyboard::Key::W) {
      castAbility(PlayerAbilities::W, event.getMousePosition());
    }
    if (event.getKeyboardEvent() == sf::Keyboard::Key::E) {
      castAbility(PlayerAbilities::E, event.getMousePosition());
    }
    if (event.getKeyboardEvent() == sf::Keyboard::Key::R) {
      castAbility(PlayerAbilities::R, event.getMousePosition());
    }
  });

  m_eventDispatcher.subscribe<CleanUpEvent>(this, [this](CleanUpEvent& event) {
    if (m_currentHealth <= 0) {
      m_alive = false;
      m_eventDispatcher.unsubscribe(this);
    };
  });
}

void Player::onAnimationEnd() {
  if (m_state != PlayerAnimationState::IDLE) {
    setAnimation(PlayerAnimationState::IDLE);
  } else {
    m_currentFrame = m_startFrame;
  }
}

void Player::onDraw(DrawEvent& event) {
  sf::RenderWindow& window = event.getWindow();
  window.draw(m_sprite);

  drawAbilities(window);

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
  }
}

void Player::castAbility(PlayerAbilities ability, sf::Vector2f mousePosition) {
  switch (ability) {
    case PlayerAbilities::Q:
      m_activeAbilities.emplace_back(
          m_qTexture, 64, 128, 10, 0.1f, 6, AbilityLocationData{{16.f, 16.f}, mousePosition, {0.f, 0.f}, {0.f, 120.f}});
      break;
    case PlayerAbilities::W:
      m_activeAbilities.emplace_back(m_wTexture,
                                     64,
                                     64,
                                     14,
                                     0.1f,
                                     12,
                                     AbilityLocationData{{48.f, 48.f}, mousePosition, {0.f, 40.f}, {0.f, 40.f}},
                                     4.f);
      break;
    case PlayerAbilities::E:
      m_activeAbilities.emplace_back(m_eTexture, 32, 32, 7, 0.1f, 6, AbilityLocationData{{64.f, 32.f}, mousePosition});
      break;
    case PlayerAbilities::R:
      m_activeAbilities.emplace_back(m_rTexture,
                                     64,
                                     88,
                                     11,
                                     0.1f,
                                     7,
                                     AbilityLocationData{{48.f, 32.f}, mousePosition, {0.f, 0.f}, {0.f, 120.f}},
                                     4.f);
      break;
    default:
      break;
  }
}

void Player::updateAbilities(float deltaTime) {
  for (std::vector<Ability>::iterator it = m_activeAbilities.begin(); it != m_activeAbilities.end();) {
    it->update(deltaTime);
    if (it->isInDmgFrame() && !it->hasEmitted()) {
      AbilityDmgEvent event(it->getHitbox(), it->getDmg());
      m_eventDispatcher.emit(event);
      it->emit();
    }
    if (!it->isActive()) {
      it = m_activeAbilities.erase(it);
    } else {
      ++it;
    }
  }
}

void Player::drawAbilities(sf::RenderWindow& window) {
  for (Ability& spell : m_activeAbilities) {
    spell.draw(window);

    if (spell.isInDmgFrame() && Config::Settings::showHitboxes) {
      sf::CircleShape attackHitboxShape;

      float radius = std::min(spell.getHitbox().size.x, spell.getHitbox().size.y) / 2.f;
      attackHitboxShape.setRadius(radius);

      attackHitboxShape.setPosition({spell.getHitbox().position.x + spell.getHitbox().size.x / 2.f - radius,
                                     spell.getHitbox().position.y + spell.getHitbox().size.y / 2.f - radius});

      attackHitboxShape.setFillColor(sf::Color::Transparent);
      attackHitboxShape.setOutlineColor(sf::Color::Red);
      attackHitboxShape.setOutlineThickness(2.f);

      window.draw(attackHitboxShape);
    }
  }
}

void Player::setAnimation(PlayerAnimationState animationState) {
  if (m_animationConfigs.count(animationState) > 0) {
    const AnimationConfig& config = m_animationConfigs.at(animationState);
    m_startFrame = config.startFrame;
    m_endFrame = config.endFrame;
    m_frameTime = config.frameTime;
    m_currentFrame = m_startFrame;
  }
  m_state = animationState;
}

void Player::setWalking() {
  if (m_state != PlayerAnimationState::WALKING) {
    setAnimation(PlayerAnimationState::WALKING);
  }
}

void Player::setIdle() {
  if (m_state != PlayerAnimationState::IDLE) {
    setAnimation(PlayerAnimationState::IDLE);
  }
}

void Player::onUpdate(const float deltaTime) {
  updateHealthBar();
  updateHitbox();
  updateAnimation(deltaTime);
  updateAbilities(deltaTime);

  if (isHitting()) {
    if (m_state == PlayerAnimationState::AA1 || m_state == PlayerAnimationState::AA2) {
      if (m_currentFrame == m_animationConfigs.at(m_state).dmgFrame) {
        std::shared_ptr<Entity> target = getTarget();
        if (target) {
          target->takeDmg(m_physicalDmg);
        }
      }
    }
    return;
  }

  if (hasTarget()) {
    std::shared_ptr<Entity> target = getTarget();
    if (target) {
      setDestination(target->getHitbox().getCenter());
      if (isTargetInRange(target)) {
        onTargetInRange();
      }
    }
  }

  if (!isHitting()) {
    move(deltaTime);
  }
}

void Player::updateHealthBar() {
  float healthPercentage = m_currentHealth / m_maxHealth;
  m_healthBarForeground.setSize(sf::Vector2f(healthPercentage * 100.f, 10.f));

  sf::FloatRect bounds = m_sprite.getGlobalBounds();
  float healthBarX = bounds.position.x + (bounds.size.x / 2.f) - (m_healthBarBackground.getSize().x / 2.f);
  float healthBarY = bounds.position.y - m_healthBarBackground.getSize().y + 25.f;
  m_healthBarBackground.setPosition({healthBarX, healthBarY});
  m_healthBarForeground.setPosition({healthBarX, healthBarY});
}

void Player::updateHitbox() {
  sf::FloatRect spriteBounds = m_sprite.getGlobalBounds();

  float hitboxWidth = spriteBounds.size.x * 0.3f;
  float hitboxHeight = spriteBounds.size.y * 0.4f;

  float hitboxLeft = spriteBounds.position.x + (spriteBounds.size.x - hitboxWidth) / 2.f;
  float hitboxTop = spriteBounds.position.y + (spriteBounds.size.y - hitboxHeight) / 2.f;

  m_hitbox = sf::FloatRect({hitboxLeft, hitboxTop}, {hitboxWidth, hitboxHeight});
}

void Player::onTargetInRange() {
  autoAttack();
}

void Player::autoAttack() {
  if (Utils::getRandomInt(0, 1) == 1) {
    setAnimation(PlayerAnimationState::AA1);
  } else {
    setAnimation(PlayerAnimationState::AA2);
  }
}

bool Player::isHitting() const {
  return (m_state != PlayerAnimationState::IDLE && m_state != PlayerAnimationState::WALKING);
}
