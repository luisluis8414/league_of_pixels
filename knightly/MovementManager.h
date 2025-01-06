#pragma once

#include "Entity.h"
#include "Event.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>
#include <memory>
#include "Minion.h"

class MovementManager {
public:
    MovementManager(EventDispatcher& dispatcher, const Player& player,
        const std::vector<std::shared_ptr<Enemy>>& enemies, const std::vector<std::shared_ptr<Minion>>& blueSideMinions, const std::vector<std::shared_ptr<Minion>>& redSideMinions);

    ~MovementManager() = default;

    void checkCollisions();


private:
    EventDispatcher& m_eventDispatcher;
    const Player& m_player;
    const std::vector<std::shared_ptr<Enemy>>& m_enemies;
    const std::vector<std::shared_ptr<Minion>>& m_blueSideMinions;
    const std::vector<std::shared_ptr<Minion>>& m_redSideMinions;

    void handleEntityMove(sf::Sprite& sprite, const sf::FloatRect& hitbox, const sf::Vector2f& step, sf::Vector2f& destination);
    void checkForTarget(sf::Vector2f position);
};
