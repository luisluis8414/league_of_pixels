#pragma once
#include <SFML/Graphics/Rect.hpp>
#include <random>
#include <string>

namespace Utils {
inline int getRandomInt(int min, int max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(min, max);
  return dis(gen);
}

inline float getRandomFloat(float min, float max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(min, max);
  return dis(gen);
}

// Checks for axis-aligned bounding box (AABB) collision
inline bool aabbCollision(const sf::FloatRect& a, const sf::FloatRect& b) {
  return (a.position.x + a.size.x > b.position.x && a.position.x < b.position.x + b.size.x &&
          a.position.y + a.size.y > b.position.y && a.position.y < b.position.y + b.size.y);
};
}  // namespace Utils