#include "Map.h"
#include <stdexcept>

namespace Map {
    Rift::Rift(EventDispatcher& dispatcher) : m_eventDispatcher(dispatcher) {
        if (!m_texture.loadFromFile(Map::filePath)) {
            throw std::runtime_error("Failed to load texture from " + Map::filePath);
        }

        m_waterVertices.setPrimitiveType(sf::PrimitiveType::Triangles);
        m_tileVertices.setPrimitiveType(sf::PrimitiveType::Triangles);

        m_tileVertices.resize(Map::IslandMap.size() * Map::IslandMap[0].size() * 6); // 6 vertices per tile

        for (unsigned int row = 0; row < Map::IslandMap.size(); ++row) {
            for (unsigned int col = 0; col < Map::IslandMap[row].size(); ++col) {
                const int& tileId = Map::IslandMap[row][col];
                if (tileId == 0) continue; // skip water tiles now

                // 6 vertices per tile
                sf::Vertex* triangle = &m_tileVertices[(row * Map::IslandMap[row].size() + col) * 6];

                const float left = static_cast<float>(col * Map::tileSize);
                const float right = static_cast<float>((col + 1) * Map::tileSize);
                const float top = static_cast<float>(row * Map::tileSize);
                const float bottom = static_cast<float>((row + 1) * Map::tileSize);

                // first triangle
                triangle[0].position = sf::Vector2f(left, top);
                triangle[1].position = sf::Vector2f(right, top);
                triangle[2].position = sf::Vector2f(right, bottom);

                // second triangle
                triangle[3].position = sf::Vector2f(left, top);
                triangle[4].position = sf::Vector2f(right, bottom);
                triangle[5].position = sf::Vector2f(left, bottom);

                const sf::IntRect rect = Map::tileRects.at(tileId);

                // texture coordinates for first triangle
                triangle[0].texCoords = sf::Vector2f(static_cast<float>(rect.position.x), static_cast<float>(rect.position.y));
                triangle[1].texCoords = sf::Vector2f(static_cast<float>(rect.position.x + rect.size.x), static_cast<float>(rect.position.y));
                triangle[2].texCoords = sf::Vector2f(static_cast<float>(rect.position.x + rect.size.x), static_cast<float>(rect.position.y + rect.size.y));

                // texture coordinates for second triangle
                triangle[3].texCoords = sf::Vector2f(static_cast<float>(rect.position.x), static_cast<float>(rect.position.y));
                triangle[4].texCoords = sf::Vector2f(static_cast<float>(rect.position.x + rect.size.x), static_cast<float>(rect.position.y + rect.size.y));
                triangle[5].texCoords = sf::Vector2f(static_cast<float>(rect.position.x), static_cast<float>(rect.position.y + rect.size.y));
            }
        }


        m_eventDispatcher.subscribe<DrawEvent>(this, [this](DrawEvent& event) {
            draw(event.getWindow());
            }, RenderLayer::MAP);
    }

    void Rift::draw(sf::RenderWindow& window) {
        sf::RenderStates states;
        states.texture = &m_texture;

        sf::View currentView = window.getView();
        sf::FloatRect viewBounds(
            { currentView.getCenter().x - currentView.getSize().x / 2.0f,
            currentView.getCenter().y - currentView.getSize().y / 2.0f, }
            ,{ currentView.getSize().x,
            currentView.getSize().y }
        );

        m_waterVertices.clear();
        m_waterVertices.resize(6); // 6 vertices for water (1 quad as 2 triangles)

        sf::Vertex* triangle = &m_waterVertices[0];

        // calculate view bounds
        const float left = viewBounds.position.x;
        const float right = viewBounds.position.x + viewBounds.size.x;
        const float top = viewBounds.position.y;
        const float bottom = viewBounds.position.y + viewBounds.size.y;

        // first triangle
        triangle[0].position = sf::Vector2f(left, top);
        triangle[1].position = sf::Vector2f(right, top);
        triangle[2].position = sf::Vector2f(right, bottom);

        // second triangle
        triangle[3].position = sf::Vector2f(left, top);
        triangle[4].position = sf::Vector2f(right, bottom);
        triangle[5].position = sf::Vector2f(left, bottom);

        const sf::IntRect waterRect = Map::tileRects.at(0);

        // texture coordinates for first triangle
        triangle[0].texCoords = sf::Vector2f(static_cast<float>(waterRect.position.x), static_cast<float>(waterRect.position.y));
        triangle[1].texCoords = sf::Vector2f(static_cast<float>(waterRect.position.x + waterRect.size.x), static_cast<float>(waterRect.position.y));
        triangle[2].texCoords = sf::Vector2f(static_cast<float>(waterRect.position.x + waterRect.size.x), static_cast<float>(waterRect.position.y + waterRect.size.y));

        // texture coordinates for second triangle
        triangle[3].texCoords = sf::Vector2f(static_cast<float>(waterRect.position.x), static_cast<float>(waterRect.position.y));
        triangle[4].texCoords = sf::Vector2f(static_cast<float>(waterRect.position.x + waterRect.size.x), static_cast<float>(waterRect.position.y + waterRect.size.y));
        triangle[5].texCoords = sf::Vector2f(static_cast<float>(waterRect.position.x), static_cast<float>(waterRect.position.y + waterRect.size.y));


        window.draw(m_waterVertices, states);

        window.draw(m_tileVertices, states);
    }

}
