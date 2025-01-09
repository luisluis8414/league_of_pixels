#include "MovementManager.h"
#include <iostream>
#include <vector>
#include "Event.h"
#include "Map.h"
#include "Utils.h"

EntityManager::EntityManager(EventDispatcher& dispatcher, const Player& player, const std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<std::shared_ptr<Minion>>& blueSideMinions, const std::vector<std::shared_ptr<Minion>>& redSideMinions)
	: m_eventDispatcher(dispatcher), m_player(player), m_enemies(enemies), m_blueSideMinions(blueSideMinions), m_redSideMinions(redSideMinions) {

	m_eventDispatcher.subscribe<MoveEvent>(this, [this](MoveEvent& moveEvent) {
		handleEntityMove(moveEvent.getSprite(), moveEvent.getHitbox(), moveEvent.getStep(), moveEvent.getDestination());
		});

	m_eventDispatcher.subscribe<TickEvent>(this, [this](TickEvent& event) {
		checkCollisions();
		});

	m_eventDispatcher.subscribe<MouseRightClickEvent>(this, [this](MouseRightClickEvent event) {
		checkForTarget(event.getPosition());
		});

	m_eventDispatcher.subscribe<AbilityDmgEvent>(this, [this](AbilityDmgEvent event) {
		checkAbilityDmg(event.getHitbox(), event.getSpellDmg());
		});
}

void EntityManager::checkAbilityDmg(sf::FloatRect hitbox, float spellDmg) {
	for (const std::shared_ptr<Minion>& redSideMinion : m_redSideMinions) {
		if (Utils::aabbCollision(redSideMinion->getHitbox(), hitbox)) {
			redSideMinion->takeDmg(spellDmg);
			break;
		}
	}
}

void EntityManager::checkForTarget(sf::Vector2f position) {
	Entity* target = nullptr;

	for (const std::shared_ptr<Minion>& redSideMinion : m_redSideMinions) {
		if (redSideMinion->getHitbox().contains(position)) {
			target = redSideMinion.get();
			break;
		}
	}

	ActionEventType actionType = ActionEventType::MoveAction;

	if (target) actionType = ActionEventType::TargetAction;

	ActionEvent actionEvent(actionType, m_player, target, position);
	m_eventDispatcher.emit(actionEvent);
}

void EntityManager::checkCollisions() {
	for (const std::shared_ptr<Enemy>& enemy : m_enemies) {
		if (m_player.isHitting() && Utils::aabbCollision(m_player.getAttackHitbox(), enemy->getHitbox())) {
			CollisionEvent collisionEvent(m_player, *enemy);
			m_eventDispatcher.emit(collisionEvent);
		}
	}

	for (const std::shared_ptr<Minion>& redSideMinion : m_redSideMinions) {
		if (m_player.isHitting() && Utils::aabbCollision(m_player.getAttackHitbox(), redSideMinion->getHitbox())) {
			CollisionEvent collisionEvent(m_player, *redSideMinion);
			m_eventDispatcher.emit(collisionEvent);
		}
	}
}



// handles a move for an entity and does terrain collision check
void EntityManager::handleEntityMove(sf::Sprite& sprite, const sf::FloatRect& hitbox, const sf::Vector2f& step, sf::Vector2f& destination) {
	sf::FloatRect newXHitbox = hitbox;
	newXHitbox.position.x += step.x;

	sf::FloatRect newYHitbox = hitbox;
	newYHitbox.position.y += step.y;

	bool canMoveX = Map::isTileWalkable(newXHitbox);
	bool canMoveY = Map::isTileWalkable(newYHitbox);

	if (canMoveX) {
		sprite.move({ step.x, 0.f });
	}
	else {
		destination.x = sprite.getPosition().x;
	}

	if (canMoveY) {
		sprite.move({ 0.f, step.y });
	}
	else {
		destination.y = sprite.getPosition().y;
	}
}

