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
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dis(min, max);
  return dis(gen);
}

// Checks for axis-aligned bounding box (AABB) collision
inline static bool aabbCollision(const sf::FloatRect& a, const sf::FloatRect& b) {
  return (a.position.x + a.size.x > b.position.x && a.position.x < b.position.x + b.size.x &&
          a.position.y + a.size.y > b.position.y && a.position.y < b.position.y + b.size.y);
};

inline static bool isRectInCircle(const sf::FloatRect& hitbox, const sf::CircleShape& range) {
  sf::Vector2f circleCenter = range.getPosition();
  float radius = range.getRadius();

  sf::Vector2f rectCenter(hitbox.position.x + hitbox.size.x / 2.f, hitbox.position.y + hitbox.size.y / 2.f);

  float closestX = std::max(hitbox.position.x, std::min(circleCenter.x, hitbox.position.x + hitbox.size.x));
  float closestY = std::max(hitbox.position.y, std::min(circleCenter.y, hitbox.position.y + hitbox.size.y));

  float dx = circleCenter.x - closestX;
  float dy = circleCenter.y - closestY;

  if ((dx * dx + dy * dy) <= (radius * radius)) {
    return true;
  }
  return false;
};

inline static float getVectorDistance(const sf::Vector2f& pos1, const sf::Vector2f& pos2) {
  float dx = pos1.x - pos2.x;
  float dy = pos1.y - pos2.y;
  return std::sqrt(dx * dx + dy * dy);
}
}  // namespace Utils
