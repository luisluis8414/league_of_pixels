#include "CollisionSystem.h"
#include <iostream>
#include <vector>
#include "Event.h"
#include "Map.h"


CollisionSystem::CollisionSystem(EventDispatcher& dispatcher, const Player& player, const std::vector<std::unique_ptr<Enemy>>& enemies)
	: m_eventDispatcher(dispatcher), m_player(player), m_enemies(enemies) {

	m_eventDispatcher.subscribe<MoveEvent>(this, [this](MoveEvent& moveEvent) {
		handleEntityMove(moveEvent.getSprite(), moveEvent.getHitbox(), moveEvent.getDestination(), [&moveEvent]() { moveEvent.clearDestination(); });
		});

	m_eventDispatcher.subscribe<TickEvent>(this, [this](TickEvent& event) {
		update();
		});
}

void CollisionSystem::update() {
	// Check collisions between player and each enemy
	for (const auto& enemy : m_enemies) {
		// If the player is hitting and collides with an enemy
		if (m_player.isHitting() && aabbCollision(m_player.getAttackHitbox(), enemy->getHitbox())) {
			CollisionEvent collisionEvent(m_player, *enemy);
			m_eventDispatcher.emit(collisionEvent);
		}

		// Uncomment if enemies can hit the player
		/*
		if (enemy->isHitting() && aabbCollision(enemy->getAttackHitbox(), m_player.getHitbox())) {
			CollisionEvent collisionEvent(*enemy, m_player);
			m_eventDispatcher.emit(collisionEvent);
		}
		*/
	}

	// Uncomment if enemy-to-enemy collisions are necessary
	/*
	for (size_t i = 0; i < m_enemies.size(); ++i) {
		for (size_t j = i + 1; j < m_enemies.size(); ++j) {
			if (aabbCollision(m_enemies[i]->getHitbox(), m_enemies[j]->getHitbox())) {
				CollisionEvent collisionEvent(*m_enemies[i], *m_enemies[j]);
				m_eventDispatcher.emit(collisionEvent);
			}
		}
	}
	*/
}

bool CollisionSystem::aabbCollision(const sf::FloatRect& a, const sf::FloatRect& b) const {
	return (a.left + a.width > b.left &&
		a.left < b.left + b.width &&
		a.top + a.height > b.top &&
		a.top < b.top + b.height);
}

void CollisionSystem::handleEntityMove(sf::Sprite& sprite, const sf::FloatRect& hitbox, const sf::Vector2f& destination, std::function<void()> clearDestination) {
	sf::FloatRect newXHitbox = hitbox;
	newXHitbox.left += destination.x;

	sf::FloatRect newYHitbox = hitbox;
	newYHitbox.top += destination.y;

	bool canMoveX = Map::isTilePassable(newXHitbox.left, newXHitbox.top) && Map::isTilePassable((newXHitbox.left + newXHitbox.width), newXHitbox.top);
	bool canMoveY = Map::isTilePassable(newYHitbox.left, newYHitbox.top) && Map::isTilePassable(newYHitbox.left, (newYHitbox.top + newYHitbox.height));

	if (canMoveX) {
		sprite.move(destination.x, 0.f);
	}

	if (canMoveY) {
		sprite.move(0.f, destination.y);
	}

	if ((!canMoveX && destination.y == 0) || (!canMoveY && destination.x == 0)) {
		clearDestination();
	}
}

