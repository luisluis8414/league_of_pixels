#include "CollisionSystem.h"
#include <iostream>
#include <vector>
#include "Event.h"
#include "Map.h"

CollisionSystem::CollisionSystem(EventDispatcher& dispatcher, const Player& player, const std::vector<std::unique_ptr<Enemy>>& enemies)
	: m_eventDispatcher(dispatcher), m_player(player), m_enemies(enemies) {

	m_eventDispatcher.subscribe<MoveEvent>(this, [this](MoveEvent& moveEvent) {
		handleEntityMove(moveEvent.getSprite(), moveEvent.getDestination());
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

void CollisionSystem::handleEntityMove(sf::Sprite& sprite, std::pair<float, float> destination) {
	float newX = destination.first + sprite.getPosition().x;
	float newY = destination.second + sprite.getPosition().y;

	sf::FloatRect bounds = sprite.getGlobalBounds();
	float spriteWidth = bounds.width;
	float spriteHeight = bounds.height;

	int gridXLeft = std::max(0, static_cast<int>(newX / Map::tileSize));
	int gridYTop = std::max(0, static_cast<int>(newY / Map::tileSize));

	int gridXRight = static_cast<int>((newX + spriteWidth) / Map::tileSize);
	int gridYBottom = static_cast<int>((newY + spriteHeight) / Map::tileSize);

	// Ensure safe indexing for +1 and -1 operations
	int safeGridYTopPlusOne = std::min(gridYTop + 1, static_cast<int>(Map::IslandMap.size() - 1));
	int safeGridXLeftPlusOne = std::min(gridXLeft + 1, static_cast<int>(Map::IslandMap[0].size() - 1));
	int safeGridYBottomMinusOne = std::max(gridYBottom - 1, 0);
	int safeGridXRightMinusOne = std::max(gridXRight - 1, 0);

	if (!((gridXLeft < 0 || gridXRight >= Map::IslandMap[0].size() ||
		gridYTop + 1 < 0 || gridYBottom >= Map::IslandMap.size()) ||
		(Map::IslandMap[safeGridYTopPlusOne][safeGridXLeftPlusOne] == 0 ||
			Map::IslandMap[safeGridYBottomMinusOne][safeGridXRightMinusOne] == 0))) {
		sprite.move(destination.first, destination.second);
	}
}
