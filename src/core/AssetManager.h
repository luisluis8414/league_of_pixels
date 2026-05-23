#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

class AssetManager {
 public:
  static AssetManager& instance() {
    static AssetManager singleton;
    return singleton;
  }

  const sf::Texture& getTexture(const std::string& path) {
    auto it = m_textures.find(path);
    if (it != m_textures.end()) return it->second;

    sf::Texture& texture = m_textures[path];
    if (!texture.loadFromFile(path)) {
      std::cerr << "Failed to load texture: " << path << std::endl;
    }
    texture.setSmooth(false);
    return texture;
  }

 private:
  AssetManager() = default;
  std::unordered_map<std::string, sf::Texture> m_textures;
};
