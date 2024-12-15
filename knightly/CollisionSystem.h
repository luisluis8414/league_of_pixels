#pragma once
#include "Entity.h"
#include <vector>
#include "Event.h"

class CollisionSystem {
public:
	CollisionSystem(EventDispatcher& dipatcher) : m_eventDisptacher(dipatcher){};

	~CollisionSystem() = default;

	void update(std::vector<std::unique_ptr<Entity>>& entities);
private:
	EventDispatcher& m_eventDisptacher;

	bool aabbCollision(const sf::FloatRect& hitboxA,const sf::FloatRect& hitboxB);
};