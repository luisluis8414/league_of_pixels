#pragma once

#include "Entity.h"
#include "Event.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>
#include <memory>

class CollisionSystem {
public:
    CollisionSystem(EventDispatcher& dispatcher, const Player& player,
        const std::vector<std::unique_ptr<Enemy>>& enemies);

    ~CollisionSystem() = default;

    void update();


private:
    EventDispatcher& m_eventDispatcher;
    const Player& m_player;
    const std::vector<std::unique_ptr<Enemy>>& m_enemies;

    // Checks for axis-aligned bounding box (AABB) collision
    bool aabbCollision(const sf::FloatRect& hitboxA, const sf::FloatRect& hitboxB) const;

    //void handleEntityMove(sf::Sprite& sprite, const sf::FloatRect& hitbox, const sf::Vector2f& destination);
};
