#include "EntityManager.h"

#include <iostream>
#include <vector>

#include "../components/Map.h"
#include "../core/Config.h"
#include "../core/Event.h"
#include "../core/Utils.h"

EntityManager::EntityManager(EventDispatcher& dispatcher,
                             std::shared_ptr<Player> player,
                             std::vector<std::shared_ptr<Enemy>>& enemies,
                             std::vector<std::shared_ptr<Minion>>& blueSideMinions,
                             std::vector<std::shared_ptr<Minion>>& redSideMinions,
                             std::vector<std::shared_ptr<Tower>>& blueSideTowers,
                             std::vector<std::shared_ptr<Tower>>& redSideTowers)
    : m_eventDispatcher(dispatcher),
      m_buildingManager(dispatcher, player, blueSideTowers, redSideTowers, blueSideMinions, redSideMinions),
      m_player(player),
      m_enemies(enemies),
      m_blueSideMinions(blueSideMinions),
      m_redSideMinions(redSideMinions),
      m_blueSideTowers(blueSideTowers),
      m_minionsManager(dispatcher, blueSideMinions, redSideMinions) {
  m_eventDispatcher.subscribe<InitEvent>(this, [this](InitEvent event) { this->init(); });

  m_eventDispatcher.subscribe<TickEvent>(this, [this](TickEvent& event) { this->checkCollisions(); });

  m_eventDispatcher.subscribe<MouseRightClickEvent>(
      this, [this](MouseRightClickEvent event) { this->checkForTarget(event.getPosition()); });

  m_eventDispatcher.subscribe<AbilityDmgEvent>(
      this, [this](AbilityDmgEvent event) { this->checkAbilityDmg(event.getHitbox(), event.getSpellDmg()); });

  m_eventDispatcher.subscribe<DestroyEntityEvent>(
      this, [this](DestroyEntityEvent& event) { m_entitiesToDestroy.push_back(event.getEntity()); });

  m_eventDispatcher.subscribe<CleanUpEvent>(this, [this](CleanUpEvent& event) { this->cleanUp(); });
}

void EntityManager::init() {
  spawnEnemy(Config::Textures::Troops::TORCH_RED, {200.f, 200.f});
  spawnEnemy(Config::Textures::Troops::TNT_RED, {200.f, 300.f});
}

void EntityManager::checkAbilityDmg(sf::FloatRect hitbox, float spellDmg) {
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

void EntityManager::checkForTarget(sf::Vector2f position) {
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

  ActionEvent actionEvent(actionType, *m_player, target, position);
  m_eventDispatcher.emit(actionEvent);
}

void EntityManager::checkCollisions() {
  for (const std::shared_ptr<Enemy>& enemy : m_enemies) {
    if (m_player->isHitting() && Utils::aabbCollision(m_player->getAttackHitbox(), enemy->getHitbox())) {
      CollisionEvent collisionEvent(*m_player, *enemy);
      m_eventDispatcher.emit(collisionEvent);
    }
  }

  for (const std::shared_ptr<Minion>& redSideMinion : m_redSideMinions) {
    if (m_player->isHitting() && Utils::aabbCollision(m_player->getAttackHitbox(), redSideMinion->getHitbox())) {
      CollisionEvent collisionEvent(*m_player, *redSideMinion);
      m_eventDispatcher.emit(collisionEvent);
    }
  }
}

void EntityManager::spawnEnemy(const std::string& texturePath, sf::Vector2f position) {
  m_enemies.push_back(std::make_shared<Enemy>(m_eventDispatcher, texturePath, position));
}

void EntityManager::cleanUp() {
  for (int i = 0; i < m_entitiesToDestroy.size(); ++i) {
       for (int j = 0; j < m_enemies.size(); ++j) {
      if (m_enemies[j].get() == m_entitiesToDestroy[i]) {
        m_enemies.erase(m_enemies.begin() + j);
        break;
      }
    }
    for (int j = 0; j < m_redSideMinions.size(); ++j) {
      if (m_redSideMinions[j].get() == m_entitiesToDestroy[i]) {
        m_redSideMinions.erase(m_redSideMinions.begin() + j);
        break;
      }
    }
    for (int j = 0; j < m_blueSideMinions.size(); ++j) {
      if (m_blueSideMinions[j].get() == m_entitiesToDestroy[i]) {
        m_blueSideMinions.erase(m_blueSideMinions.begin() + j);
        break;
      }
    }
  }
  m_entitiesToDestroy.clear();
}