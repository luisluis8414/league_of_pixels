#include "CollisionSystem.h"
#include <iostream>
#include <vector>
#include "Event.h"
#include "Player.h"

void CollisionSystem::update(std::vector<std::unique_ptr<Entity>>& entities) {
    for (size_t i = 0; i < entities.size(); ++i) {
        for (size_t j = i + 1; j < entities.size(); ++j) {
            auto& entityA = *(entities[i]);
            auto& entityB = *(entities[j]);

            if (entityA.getType() == EntityType::Player &&
                dynamic_cast<Player*>(&entityA)->isHitting() &&
                aabbCollision(dynamic_cast<Player*>(&entityA)->getAttackHitbox(), entityB.getHitbox())) {
                CollisionEvent collisionEvent(entityA, entityB); 
                m_eventDisptacher.emit(collisionEvent);
            }

            if (entityB.getType() == EntityType::Player &&
                dynamic_cast<Player*>(&entityB)->isHitting() &&
                aabbCollision(dynamic_cast<Player*>(&entityB)->getAttackHitbox(), entityA.getHitbox())) {
                CollisionEvent collisionEvent(entityB, entityA);
                m_eventDisptacher.emit(collisionEvent);
            }
        }
    }
}

bool CollisionSystem::aabbCollision(const sf::FloatRect& a, const sf::FloatRect& b) {
    return (a.left + a.width > b.left &&  
        a.left < b.left + b.width &&
        a.top + a.height > b.top &&
        a.top < b.top + b.height);  
}
