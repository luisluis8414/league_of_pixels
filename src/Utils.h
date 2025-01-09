#pragma once
#include <string>
#include <random>

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
        return (a.left + a.width > b.left &&
            a.left < b.left + b.width &&
            a.top + a.height > b.top &&
            a.top < b.top + b.height);
    };
}