#include "MovementManager.h"

#include <iostream>
#include <vector>

#include "../components/Map.h"
#include "../core/Event.h"
#include "../core/Utils.h"

MovementManager::MovementManager(EventDispatcher& dispatcher,
                                 const Player& player,
                                 const std::vector<std::shared_ptr<Enemy>>& enemies,
                                 const std::vector<std::shared_ptr<Minion>>& blueSideMinions,
                                 const std::vector<std::shared_ptr<Minion>>& redSideMinions,
                                 const std::vector<std::shared_ptr<Tower>>& blueSideTowers)
    : m_eventDispatcher(dispatcher),
      m_player(player),
      m_enemies(enemies),
      m_blueSideMinions(blueSideMinions),
      m_redSideMinions(redSideMinions),
      m_blueSideTowers(blueSideTowers) {
  m_eventDispatcher.subscribe<MoveEvent>(this, [this](MoveEvent& moveEvent) {
    handleEntityMove(moveEvent.getSprite(), moveEvent.getHitbox(), moveEvent.getStep(), moveEvent.getDestination());
  });

  m_eventDispatcher.subscribe<TickEvent>(this, [this](TickEvent& event) { checkCollisions(); });

  m_eventDispatcher.subscribe<MouseRightClickEvent>(
      this, [this](MouseRightClickEvent event) { checkForTarget(event.getPosition()); });

  m_eventDispatcher.subscribe<AbilityDmgEvent>(
      this, [this](AbilityDmgEvent event) { checkAbilityDmg(event.getHitbox(), event.getSpellDmg()); });
}

void MovementManager::checkAbilityDmg(sf::FloatRect hitbox, float spellDmg) {
  for (const std::shared_ptr<Minion>& redSideMinion : m_redSideMinions) {
    if (Utils::aabbCollision(redSideMinion->getHitbox(), hitbox)) {
      redSideMinion->takeDmg(spellDmg);
    }
  }
  for (const std::shared_ptr<Enemy>& enemy : m_enemies) {
    if (Utils::aabbCollision(enemy->getHitbox(), hitbox)) {
      enemy->takeDmg(spellDmg);
    }
  }
}

void MovementManager::checkForTarget(sf::Vector2f position) {
  Entity* target = nullptr;

  for (const std::shared_ptr<Minion>& redSideMinion : m_redSideMinions) {
    if (redSideMinion->getHitbox().contains(position)) {
      target = redSideMinion.get();
      break;
    }
  }

  for (const std::shared_ptr<Enemy>& enemy : m_enemies) {
    if (enemy->getHitbox().contains(position)) {
      target = enemy.get();
      break;
    }
  }

  ActionEventType actionType = ActionEventType::MoveAction;

  if (target) actionType = ActionEventType::TargetAction;

  ActionEvent actionEvent(actionType, m_player, target, position);
  m_eventDispatcher.emit(actionEvent);
}

void MovementManager::checkCollisions() {
  for (const std::shared_ptr<Tower>& tower : m_blueSideTowers) {
    for (const std::shared_ptr<Minion>& redSideMinion : m_redSideMinions) {
      sf::CircleShape range = tower->getRange();
      const sf::FloatRect& hitbox = redSideMinion->getHitbox();

      sf::Vector2f circleCenter = range.getPosition();
      float radius = range.getRadius();

      sf::Vector2f rectCenter(hitbox.position.x + hitbox.size.x / 2.f, hitbox.position.y + hitbox.size.y / 2.f);

      float closestX = std::max(hitbox.position.x, std::min(circleCenter.x, hitbox.position.x + hitbox.size.x));
      float closestY = std::max(hitbox.position.y, std::min(circleCenter.y, hitbox.position.y + hitbox.size.y));

      float dx = circleCenter.x - closestX;
      float dy = circleCenter.y - closestY;

      if ((dx * dx + dy * dy) <= (radius * radius)) {
        EnityInTowerRangeEvent event(*tower, *redSideMinion);
        m_eventDispatcher.emit(event);
      }
    }
  }

  for (const std::shared_ptr<Enemy>& enemy : m_enemies) {
    if (m_player.isHitting() && Utils::aabbCollision(m_player.getAttackHitbox(), enemy->getHitbox())) {
      CollisionEvent collisionEvent(m_player, *enemy);
      m_eventDispatcher.emit(collisionEvent);
    }
  }

  for (const std::shared_ptr<Minion>& redSideMinion : m_redSideMinions) {
    if (m_player.isHitting() && Utils::aabbCollision(m_player.getAttackHitbox(), redSideMinion->getHitbox())) {
      CollisionEvent collisionEvent(m_player, *redSideMinion);
      m_eventDispatcher.emit(collisionEvent);
    }
  }
}

// handles a move for an entity and does terrain collision check
void MovementManager::handleEntityMove(sf::Sprite& sprite,
                                       const sf::FloatRect& hitbox,
                                       const sf::Vector2f& step,
                                       sf::Vector2f& destination) {
  sf::FloatRect newXHitbox = hitbox;
  newXHitbox.position.x += step.x;

  sf::FloatRect newYHitbox = hitbox;
  newYHitbox.position.y += step.y;

  bool canMoveX = Map::isTileWalkable(newXHitbox);
  bool canMoveY = Map::isTileWalkable(newYHitbox);

  if (canMoveX) {
    sprite.move({step.x, 0.f});
  } else {
    destination.x = sprite.getPosition().x;
  }

  if (canMoveY) {
    sprite.move({0.f, step.y});
  } else {
    destination.y = sprite.getPosition().y;
  }
}
