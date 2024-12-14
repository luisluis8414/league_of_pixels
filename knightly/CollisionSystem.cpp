#include "CollisionSystem.h"
#include <iostream>
#include <vector>
#include "Event.h"

void CollisionSystem::update(std::vector<std::unique_ptr<Entity>>& entities) {
    for (size_t i = 0; i < entities.size(); ++i) {
        for (size_t j = i + 1; j < entities.size(); ++j) {
            auto& entityA = *(entities[i]);
            auto& entityB = *(entities[j]);

            if (checkCollision(entityA, entityB)) {
                CollisionEvent collisionEvent;
                m_eventDisptacher.emit(collisionEvent);
            }
        }
    }
}

bool CollisionSystem::checkCollision(const Entity& a, const Entity& b) {
    const sf::FloatRect& hitboxA = a.getHitbox();
    const sf::FloatRect& hitboxB = b.getHitbox();
    
    if (a.getType() != b.getType()) {
        return aabbCollision(hitboxA, hitboxB);
    }

    return false;
}


bool CollisionSystem::aabbCollision(const sf::FloatRect& a, const sf::FloatRect& b) {
    return (a.left + a.width > b.left &&  
        a.left < b.left + b.width &&
        a.top + a.height > b.top &&
        a.top < b.top + b.height);  
}
