#pragma once

#include <memory>
#include <vector>

#include "../components/buildings/Tower.h"
#include "../core/Event.h"
#include "../entities/Enemy.h"
#include "../entities/Entity.h"
#include "../entities/Minion.h"
#include "../entities/Player.h"

class MovementManager {
 public:
  MovementManager(EventDispatcher& dispatcher,
                  const Player& player,
                  const std::vector<std::shared_ptr<Enemy>>& enemies,
                  const std::vector<std::shared_ptr<Minion>>& blueSideMinions,
                  const std::vector<std::shared_ptr<Minion>>& redSideMinions,
                  const std::vector<std::shared_ptr<Tower>>& blueSideTowers);

  ~MovementManager() = default;

  void checkCollisions();

 private:
  EventDispatcher& m_eventDispatcher;
  const Player& m_player;
  const std::vector<std::shared_ptr<Enemy>>& m_enemies;
  const std::vector<std::shared_ptr<Minion>>& m_blueSideMinions;
  const std::vector<std::shared_ptr<Minion>>& m_redSideMinions;
  const std::vector<std::shared_ptr<Tower>>& m_blueSideTowers;

  void handleEntityMove(sf::Sprite& sprite,
                        const sf::FloatRect& hitbox,
                        const sf::Vector2f& step,
                        sf::Vector2f& destination);
  void checkForTarget(sf::Vector2f position);
  void checkAbilityDmg(sf::FloatRect hitbox, float spellDmg);
};
