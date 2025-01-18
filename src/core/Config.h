#pragma once
#include <string>

namespace Config {
namespace Settings {
inline static const bool showHitboxes = true;
}

namespace Fonts {
inline static const std::string ARIAL = "resources/fonts/arial.ttf";
}

namespace Textures {
namespace Buildings {
inline static const std::string RED_SIDE_NEXUS =
    "resources/tiny_swords/Factions/Knights/Buildings/Castle/Castle_Red.png";
inline static const std::string BLUE_SIDE_NEXUS =
    "resources/tiny_swords/Factions/Knights/Buildings/Castle/Castle_Blue.png";
inline static const std::string T3_BLUE_SIDE_TOWER =
    "resources/tiny_swords/Factions/Knights/Buildings/Tower/Tower_Blue.png";

inline static const std::string T3_RED_SIDE_TOWER =
    "resources/tiny_swords/Factions/Knights/Buildings/Tower/Tower_RED.png";

}  // namespace Buildings

namespace Troops {
inline static const std::string PLAYER =
    "resources/tiny_swords/Factions/Knights/Troops/Warrior/Blue/"
    "Warrior_Blue.png";

inline static const std::string TORCH_RED = "resources/tiny_swords/Factions/Goblins/Troops/Torch/Red/Torch_Red.png";
inline static const std::string TNT_RED = "resources/tiny_swords/Factions/Goblins/Troops/TNT/Red/TNT_Red.png";

inline static const std::string MINIONS_RED = "resources/minions/minionRed.png";
inline static const std::string MINIONS_BLUE = "resources/minions/minionBlue.png";

inline static const std::string BLUE_SIDE_ARCHER =
    "resources/tiny_swords/Factions/Knights/Troops/Archer/Blue/Archer_Blue.png";
inline static const std::string RED_SIDE_ARCHER =
    "resources/tiny_swords/Factions/Knights/Troops/Archer/Red/Archer_Red.png";
}  // namespace Troops

namespace Projectiles {
inline static const std::string ARROW = "resources/tiny_swords/Factions/Knights/Troops/Archer/Arrow/Arrow.png";
}

namespace Spells {
namespace Garen {
inline static const std::string Q = "resources/spells/Lightning.png";
inline static const std::string W = "resources/spells/Fire-bomb.png";
inline static const std::string E = "resources/spells/spark.png";
inline static const std::string R = "resources/spells/Dark-bolt.png";
}  // namespace Garen
}  // namespace Spells
}  // namespace Textures

namespace Window {
constexpr int WIDTH = 1920;
constexpr int HEIGHT = 1080;
inline static const std::string TITLE = "League of Pixels";

inline static const std::string ICON_PATH = "resources/leagueOfPixelsLogo.png";
}  // namespace Window

namespace Camera {
inline static const float MAX_ZOOM = 2.0f;
inline static const float MIN_ZOOM = 0.1f;

inline static const float SCOLLSPEED = 20.f;
}  // namespace Camera

namespace Minions {
inline static const float SPAWN_TIMER = 5.0f;

inline static const sf::Vector2f BLUE_SIDE_SPAWN = {500.f, 550.f};
inline static const sf::Vector2f RED_SIDE_SPAWN = {3620.f, 550.f};
}  // namespace Minions
}  // namespace Config
