#include "WorldManager.h"

#include <iostream>
#include <vector>

#include "../components/Map.h"
#include "../core/Config.h"
#include "../core/Event.h"
#include "../core/Utils.h"

WorldManager::WorldManager(EventDispatcher& dispatcher, std::shared_ptr<Player> player)
    : m_eventDispatcher(dispatcher),
      m_player(player),
      m_buildingManager(dispatcher, player, m_blueSideTowers, m_redSideTowers, m_blueSideMinions, m_redSideMinions),
      m_projectileManager(m_eventDispatcher),
      m_minionsManager(dispatcher, m_blueSideMinions, m_redSideMinions, m_blueSideTowers, m_redSideTowers) {
  m_eventDispatcher.subscribe<InitEvent>(this, [this](InitEvent event) { this->init(); });

  m_eventDispatcher.subscribe<MouseRightClickEvent>(
      this, [this](MouseRightClickEvent event) { this->checkForTarget(event.getPosition()); });

  m_eventDispatcher.subscribe<AbilityDmgEvent>(
      this, [this](AbilityDmgEvent event) { this->checkAbilityDmg(event.getHitbox(), event.getSpellDmg()); });

  m_eventDispatcher.subscribe<DestroyEntityEvent>(
      this, [this](DestroyEntityEvent& event) { m_entitiesToDestroy.push_back(event.getEntity()); });

  m_eventDispatcher.subscribe<CleanUpEvent>(this, [this](CleanUpEvent& event) { this->cleanUp(); });

  m_eventDispatcher.subscribe<DrawEvent>(
      this, [this](DrawEvent& event) { this->updateHoverHighlights(event.getWindow()); }, RenderLayer::BACKGROUND);
}

void WorldManager::init() {
  // spawnEnemy(Config::Textures::Troops::TORCH_RED, {200.f, 200.f});
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

  if (!target) {
    const std::shared_ptr<Building>& redNexus = m_buildingManager.getRedNexus();
    if (redNexus && redNexus->getHitbox().contains(position)) {
      target = redNexus;
    }
  }

  for (const std::shared_ptr<Enemy>& enemy : m_enemies) {
    enemy->setDestination(m_player->getPosition());
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

void WorldManager::updateHoverHighlights(sf::RenderWindow& window) {
  const sf::Color friendlyOutline(80, 150, 255, 190);
  const sf::Color enemyOutline(255, 80, 80, 190);
  const sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

  auto updateEntityHover = [mousePosition](const std::shared_ptr<Entity>& entity, sf::Color outlineColor) {
    entity->clearHoverOutline();
    if (entity->getHitbox().contains(mousePosition)) {
      entity->setHoverOutline(outlineColor);
    }
  };

  updateEntityHover(m_player, friendlyOutline);

  for (const std::shared_ptr<Minion>& minion : m_blueSideMinions) {
    updateEntityHover(minion, friendlyOutline);
  }
  for (const std::shared_ptr<Tower>& tower : m_blueSideTowers) {
    updateEntityHover(tower, friendlyOutline);
  }
  for (const std::shared_ptr<Minion>& minion : m_redSideMinions) {
    updateEntityHover(minion, enemyOutline);
  }
  for (const std::shared_ptr<Tower>& tower : m_redSideTowers) {
    updateEntityHover(tower, enemyOutline);
  }
  for (const std::shared_ptr<Enemy>& enemy : m_enemies) {
    updateEntityHover(enemy, enemyOutline);
  }

  if (std::shared_ptr<Entity> focused = m_player->getTarget()) {
    focused->setHoverOutline(enemyOutline);
  }
}

namespace {
template <typename T>
void eraseDead(std::vector<std::shared_ptr<T>>& v, Entity* dead) {
  auto it = std::find_if(v.begin(), v.end(), [dead](const std::shared_ptr<T>& p) { return p.get() == dead; });
  if (it != v.end()) v.erase(it);
}
}  // namespace

void WorldManager::cleanUp() {
  for (Entity* dead : m_entitiesToDestroy) {
    eraseDead(m_enemies, dead);
    eraseDead(m_redSideMinions, dead);
    eraseDead(m_blueSideMinions, dead);
    eraseDead(m_redSideTowers, dead);
  }
  m_entitiesToDestroy.clear();
}
