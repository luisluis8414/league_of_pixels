#pragma once
#include "Entity.h"
#include "Event.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>
#include <memory>


class CollisionSystem {
public:
	CollisionSystem(EventDispatcher& dipatcher) : m_eventDisptacher(dipatcher){};

	~CollisionSystem() = default;

	void update(const Player& player, const std::vector<std::unique_ptr<Enemy>>& enemys);
private:
	EventDispatcher& m_eventDisptacher;

	bool aabbCollision(const sf::FloatRect& hitboxA,const sf::FloatRect& hitboxB);
};