#pragma once

#include "../core/Event.h"
#include "../projectiles/Projectile.h"

class ProjectileManager {
 public:
  ProjectileManager(EventDispatcher& dispatcher);
  ~ProjectileManager() = default;

 private:
  EventDispatcher& m_eventDispatcher;

  std::vector<std::shared_ptr<Projectile>> m_projectiles;

  std::vector<Projectile*> m_projectilesToDestroy;

  void registerProjectile(std::shared_ptr<Projectile> projectile);

  void onTick(float deltatime);

  void cleanUp();
};