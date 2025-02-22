#include "WorldManager.h"

#include <iostream>
#include <vector>

#include "../components/Map.h"
#include "../core/Config.h"
#include "../core/Event.h"
#include "../core/Utils.h"

WorldManager::WorldManager(EventDispatcher& dispatcher, std::shared_ptr<Player> player)
    : m_eventDispatcher(dispatcher),
      m_buildingManager(dispatcher, player, m_blueSideTowers, m_redSideTowers, m_blueSideMinions, m_redSideMinions),
      m_player(player),
      m_minionsManager(dispatcher, m_blueSideMinions, m_redSideMinions, m_blueSideTowers, m_redSideTowers),
      m_projectileManager(m_eventDispatcher) {
  m_eventDispatcher.subscribe<InitEvent>(this, [this](InitEvent event) { this->init(); });

  m_eventDispatcher.subscribe<MouseRightClickEvent>(
      this, [this](MouseRightClickEvent event) { this->checkForTarget(event.getPosition()); });

  m_eventDispatcher.subscribe<AbilityDmgEvent>(
      this, [this](AbilityDmgEvent event) { this->checkAbilityDmg(event.getHitbox(), event.getSpellDmg()); });

  m_eventDispatcher.subscribe<DestroyEntityEvent>(
      this, [this](DestroyEntityEvent& event) { m_entitiesToDestroy.push_back(event.getEntity()); });

  m_eventDispatcher.subscribe<CleanUpEvent>(this, [this](CleanUpEvent& event) { this->cleanUp(); });
}

void WorldManager::init() {
  spawnEnemy(Config::Textures::Troops::TORCH_RED, {200.f, 200.f});
  spawnEnemy(Config::Textures::Troops::TNT_RED, {200.f, 300.f});
}

void WorldManager::checkAbilityDmg(sf::FloatRect hitbox, float spellDmg) {
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

void WorldManager::checkForTarget(sf::Vector2f position) {
  std::shared_ptr<Entity> target = nullptr;

  for (const std::shared_ptr<Minion>& redSideMinion : m_redSideMinions) {
    if (redSideMinion->getHitbox().contains(position)) {
      target = redSideMinion;
      break;
    }
  }

  for (const std::shared_ptr<Enemy>& enemy : m_enemies) {
    if (enemy->getHitbox().contains(position)) {
      target = enemy;
      break;
    }
  }

  for (const std::shared_ptr<Tower>& tower : m_redSideTowers) {
    if (tower->getHitbox().contains(position)) {
      target = tower;
      break;
    }
  }

  if (target) {
    m_player->setTarget(target);
  } else {
    m_player->clearTarget();
    m_player->setDestination(position);
  }
}

void WorldManager::spawnEnemy(const std::string& texturePath, sf::Vector2f position) {
  m_enemies.push_back(std::make_shared<Enemy>(m_eventDispatcher, texturePath, position));
}

void WorldManager::cleanUp() {
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
    for (int j = 0; j < m_redSideTowers.size(); ++j) {
      if (m_redSideTowers[j].get() == m_entitiesToDestroy[i]) {
        m_redSideTowers.erase(m_redSideTowers.begin() + j);
        break;
      }
    }
  }
  m_entitiesToDestroy.clear();
}