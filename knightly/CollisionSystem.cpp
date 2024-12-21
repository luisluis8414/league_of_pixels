#include "CollisionSystem.h"
#include <iostream>
#include <vector>
#include "Event.h"

void CollisionSystem::update(const Player& player, const std::vector<std::unique_ptr<Enemy>>& enemys) {
    // check collisions between player and each enemy
    for (const std::unique_ptr<Enemy>& enemy : enemys) {
        // if player is hitting and collides with an enemy
        if (player.isHitting() && aabbCollision(player.getAttackHitbox(), enemy->getHitbox())) {
            CollisionEvent collisionEvent(player, *enemy);
            m_eventDisptacher.emit(collisionEvent);
        }

        // if the enemy is hitting and collides with the player
     /*   if (enemy->isHitting() && aabbCollision(enemy->getAttackHitbox(), player.getHitbox())) {
            CollisionEvent collisionEvent(*enemy, player);
            m_eventDisptacher.emit(collisionEvent);
        }*/
    }

    // check collisions between enemies
   /* for (size_t i = 0; i < enemys.size(); ++i) {
        for (size_t j = i + 1; j < enemys.size(); ++j) {
            if (aabbCollision(enemys[i]->getHitbox(), enemys[j]->getHitbox())) {
                CollisionEvent collisionEvent(*enemys[i], *enemys[j]);
                m_eventDisptacher.emit(collisionEvent);
            }
        }
    }*/
}


bool CollisionSystem::aabbCollision(const sf::FloatRect& a, const sf::FloatRect& b) {
    return (a.left + a.width > b.left &&  
        a.left < b.left + b.width &&
        a.top + a.height > b.top &&
        a.top < b.top + b.height);  
}
