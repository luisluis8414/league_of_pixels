#include "Map.h"
#include <stdexcept>


namespace Map {

    Rift::Rift() {
        if (!m_texture.loadFromFile(Map::filePath)) {
            throw std::runtime_error("Failed to load texture from " + Map::filePath);
        }

        m_waterVertices.setPrimitiveType(sf::Quads);
        m_waterVertices.resize(Map::IslandMap.size() * Map::IslandMap[0].size() * 4);

        m_tileVertices.setPrimitiveType(sf::Quads);
        m_tileVertices.resize(Map::IslandMap.size() * Map::IslandMap[0].size() * 4);

        // drawing water everywhere first
        for (unsigned int row = 0; row < Map::IslandMap.size(); ++row) {
            for (unsigned int col = 0; col < Map::IslandMap[row].size(); ++col) {
                sf::Vertex* quad = &m_waterVertices[(row * Map::IslandMap[row].size() + col) * 4];

                const float left = col * Map::tileSize;
                const float right = (col + 1) * Map::tileSize;
                const float top = row * Map::tileSize;
                const float bottom = (row + 1) * Map::tileSize;

                quad[0].position = sf::Vector2f(left, top);
                quad[1].position = sf::Vector2f(right, top);
                quad[2].position = sf::Vector2f(right, bottom);
                quad[3].position = sf::Vector2f(left, bottom);

                const sf::IntRect waterRect = Map::tileRects.at(0); // water tile rect
                quad[0].texCoords = sf::Vector2f(waterRect.left, waterRect.top);
                quad[1].texCoords = sf::Vector2f(waterRect.left + waterRect.width, waterRect.top);
                quad[2].texCoords = sf::Vector2f(waterRect.left + waterRect.width, waterRect.top + waterRect.height);
                quad[3].texCoords = sf::Vector2f(waterRect.left, waterRect.top + waterRect.height);
            }
        }

        for (unsigned int row = 0; row < Map::IslandMap.size(); ++row) {
            for (unsigned int col = 0; col < Map::IslandMap[row].size(); ++col) {
                const int& tileId = Map::IslandMap[row][col];
                if (tileId == 0) continue; // skip water tiles now

                sf::Vertex* quad = &m_tileVertices[(row * Map::IslandMap[row].size() + col) * 4];

                const float left = col * Map::tileSize;
                const float right = (col + 1) * Map::tileSize;
                const float top = row * Map::tileSize;
                const float bottom = (row + 1) * Map::tileSize;

                quad[0].position = sf::Vector2f(left, top);
                quad[1].position = sf::Vector2f(right, top);
                quad[2].position = sf::Vector2f(right, bottom);
                quad[3].position = sf::Vector2f(left, bottom);

                const sf::IntRect rect = Map::tileRects.at(tileId);
                quad[0].texCoords = sf::Vector2f(rect.left, rect.top);
                quad[1].texCoords = sf::Vector2f(rect.left + rect.width, rect.top);
                quad[2].texCoords = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
                quad[3].texCoords = sf::Vector2f(rect.left, rect.top + rect.height);
            }
        }
    }

    void Rift::draw(sf::RenderWindow& window) {
        sf::RenderStates states;
        states.texture = &m_texture;

        window.draw(m_waterVertices, states);
        window.draw(m_tileVertices, states);
    }
}
