#include "ProjectileManager.h"

ProjectileManager::ProjectileManager(EventDispatcher& dispatcher) : m_eventDispatcher(dispatcher) {
  m_eventDispatcher.subscribe<RegisterProjectileEvent>(
      this, [this](RegisterProjectileEvent& event) { this->registerProjectile(event.getProjectile()); });
  m_eventDispatcher.subscribe<TickEvent>(this, [this](TickEvent& event) { this->onTick(event.getDeltaTime()); });

  m_eventDispatcher.subscribe<DestroyProjectileEvent>(
      this, [this](DestroyProjectileEvent& event) { m_projectilesToDestroy.push_back(event.getProjectile()); });

  m_eventDispatcher.subscribe<CleanUpEvent>(this, [this](CleanUpEvent& event) { this->cleanUp(); });
};

void ProjectileManager::registerProjectile(std::shared_ptr<Projectile> projectile) {
  m_projectiles.push_back(projectile);
};

void ProjectileManager::onTick(float deltaTime) {
  for (std::vector<std::shared_ptr<Projectile>>::iterator it = m_projectiles.begin(); it != m_projectiles.end();) {
    std::shared_ptr<Projectile>& projectile = *it;

    projectile->update(deltaTime);

    ++it;
  }
};

void ProjectileManager::cleanUp() {
  for (int i = 0; i < m_projectilesToDestroy.size(); ++i) {
    for (int j = 0; j < m_projectiles.size(); ++j) {
      if (m_projectiles[j].get() == m_projectilesToDestroy[i]) {
        m_projectiles.erase(m_projectiles.begin() + j);
        break;
      }
    }
  }
  m_projectilesToDestroy.clear();
};