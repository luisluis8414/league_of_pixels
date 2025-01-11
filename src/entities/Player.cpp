#include "Player.h"

#include "../core/Config.h"
#include "../core/Utils.h"

Player::Player(EventDispatcher& dispatcher,
               sf::Vector2f position,
               std::string qTexturePath,
               std::string wTexturePath,
               std::string eTexturePath,
               std::string rTexturePath)
    : Entity(dispatcher,
             192,
             192,
             position,
             0.1f,
             200.f,
             100.f,
             10.f,
             EntityType::Player,
             Config::Textures::Troops::PLAYER),
      m_state(PlayerAnimationState::IDLE) {
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

  m_dispatcher.subscribe<DrawEvent>(this, [this](DrawEvent& event) { onDraw(event); }, RenderLayer::PLAYER);

  m_dispatcher.subscribe<TickEvent>(this, [this](TickEvent& event) { onUpdate(event.getDeltaTime()); });

  m_dispatcher.subscribe<DestroyEntityEvent>(this, [this](DestroyEntityEvent& event) {
    if (m_target.has_value() && m_target.value() == event.getEntity()) {
      m_target.reset();
      setDestination(m_sprite.getPosition());
    }
  });

  m_dispatcher.subscribe<ActionEvent>(this, [this](ActionEvent& event) {
    if (event.getActionType() == ActionEventType::TargetAction && event.getTarget()) {
      if (m_target != event.getTarget()) {
        setAnimation(PlayerAnimationState::WALKING);
      }
      m_target = event.getTarget();
    } else {
      setDestination(event.getPosition());
      if (m_target.has_value()) m_target.reset();
    }
  });

  m_dispatcher.subscribe<KeyPressedEvent>(this, [this](KeyPressedEvent& event) {
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
}

void Player::updateAnimation(float deltaTime) {
  m_elapsedTime += deltaTime;

  if (m_elapsedTime >= m_frameTime) {
    m_elapsedTime = 0.f;

    m_currentFrame++;
    if (m_currentFrame > m_endFrame) {
      if (m_state != PlayerAnimationState::IDLE) {
        setAnimation(PlayerAnimationState::IDLE);
      } else {
        m_currentFrame = m_startFrame;
      }
    }

    if (m_state == PlayerAnimationState::AA1 || m_state == PlayerAnimationState::AA2) {
      if (m_currentFrame == m_animationConfigs.at(m_state).dmgFrame) {
        if (m_target.has_value()) {
          m_target.value()->takeDmg(m_physicalDmg);
        }
      }
    }

    int column = m_currentFrame % (m_texture.getSize().x / m_frameWidth);
    int row = m_currentFrame / (m_texture.getSize().x / m_frameWidth);
    m_frameRect = sf::IntRect({column * m_frameWidth, row * m_frameHeight}, {m_frameWidth, m_frameHeight});
    m_sprite.setTextureRect(m_frameRect);
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
      m_dispatcher.emit(event);
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

      // Set the radius based on the hitbox dimensions (assume a circular hitbox fits within the rectangle dimensions)
      float radius = std::min(spell.getHitbox().size.x, spell.getHitbox().size.y) / 2.f;
      attackHitboxShape.setRadius(radius);

      // Position the circle, adjusting for the radius to center it
      attackHitboxShape.setPosition({spell.getHitbox().position.x + spell.getHitbox().size.x / 2.f - radius,
                                     spell.getHitbox().position.y + spell.getHitbox().size.y / 2.f - radius});

      // Style the circle
      attackHitboxShape.setFillColor(sf::Color::Transparent);
      attackHitboxShape.setOutlineColor(sf::Color::Red);
      attackHitboxShape.setOutlineThickness(2.f);

      // Draw the circle
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

void Player::move(float deltaTime) {
  if (isHitting()) return;
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

    if (m_state != PlayerAnimationState::WALKING) {
      setAnimation(PlayerAnimationState::WALKING);
    }
  } else {
    if (m_state != PlayerAnimationState::IDLE) {
      setAnimation(PlayerAnimationState::IDLE);
    }
  }
}

void Player::onUpdate(const float deltaTime) {
  updateHealthBar();
  updateHitbox();
  updateAnimation(deltaTime);
  updateAbilities(deltaTime);

  if (isHitting()) return;

  if (m_target.has_value()) {
    setDestination(m_target.value()->getPosition());
    checkTargetInRange();
  }

  move(deltaTime);

  /*   if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
         setAnimation(PlayerAnimationState::SlashUp);
     }

     if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
         setAnimation(PlayerAnimationState::SlashUp);
     }*/

  /* if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
       setAnimation(PlayerAnimationState::SlashBehindLeft);
   }*/
}

void Player::updateHealthBar() {
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

void Player::updateHitbox() {
  sf::FloatRect spriteBounds = m_sprite.getGlobalBounds();

  float hitboxWidth = spriteBounds.size.x * 0.3f;
  float hitboxHeight = spriteBounds.size.y * 0.4f;

  float hitboxLeft = spriteBounds.position.x + (spriteBounds.size.x - hitboxWidth) / 2.f;
  float hitboxTop = spriteBounds.position.y + (spriteBounds.size.y - hitboxHeight) / 2.f;

  m_hitbox = sf::FloatRect({hitboxLeft, hitboxTop}, {hitboxWidth, hitboxHeight});

  // attack hitbox based on animation state
  // if (isHitting() && m_attackHitboxConfigs.count(m_state) > 0) {
  //    const auto& config = m_attackHitboxConfigs.at(m_state);

  //    // check if current frame is within the last two frames of the animation
  //    if (m_currentFrame >= (m_endFrame - 2)) {
  //        float attackHitboxWidth = spriteBounds.width * config.widthFactor;
  //        float attackHitboxHeight = spriteBounds.height * config.heightFactor;

  //        float attackHitboxLeft = spriteBounds.left + spriteBounds.width * config.offsetXFactor;
  //        float attackHitboxTop = spriteBounds.top + spriteBounds.height * config.offsetYFactor;

  //        // flip the attack hitbox if facing left
  //        float direction = (m_sprite.getScale().x < 0.f) ? -1.f : 1.f;
  //        if (direction < 0.f) {
  //            float centerX = spriteBounds.left + spriteBounds.width * 0.5f;
  //            float offsetFromCenter = attackHitboxLeft - centerX;
  //            attackHitboxLeft = centerX - offsetFromCenter - attackHitboxWidth;
  //        }

  //        m_attackHitbox = sf::FloatRect(attackHitboxLeft, attackHitboxTop, attackHitboxWidth, attackHitboxHeight);
  //    }

  //}
  // else {
  //    // reset when not attacking
  //    m_attackHitbox = sf::FloatRect(0.f, 0.f, 0.f, 0.f);
  //}
}

void Player::checkTargetInRange() {
  if (!m_target.has_value()) return;

  Entity* target = m_target.value();

  const sf::Rect targetHitbox = target->getHitbox();
  const sf::Rect playerHitbox = getHitbox();

  if (Utils::aabbCollision(playerHitbox, targetHitbox)) {
    autoAttack();
  }
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
