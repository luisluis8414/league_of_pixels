#pragma once
#include <string>

namespace Config {
namespace Settings {
inline const bool showHitboxes = true;
}

namespace Fonts {
inline const std::string ARIAL = "resources/fonts/arial.ttf";
}

namespace Textures {
namespace Buildings {
inline const std::string RED_SIDE_NEXUS = "resources/tiny_swords/Factions/Knights/Buildings/Castle/Castle_Red.png";
inline const std::string BLUE_SIDE_NEXUS = "resources/tiny_swords/Factions/Knights/Buildings/Castle/Castle_Blue.png";
inline const std::string T3_BLUE_SIDE_TOWER = "resources/tiny_swords/Factions/Knights/Buildings/Tower/Tower_Blue.png";

inline const std::string T3_RED_SIDE_TOWER = "resources/tiny_swords/Factions/Knights/Buildings/Tower/Tower_RED.png";

}  // namespace Buildings

namespace Troops {
inline const std::string PLAYER =
    "resources/tiny_swords/Factions/Knights/Troops/Warrior/Blue/"
    "Warrior_Blue.png";

inline const std::string TORCH_RED = "resources/tiny_swords/Factions/Goblins/Troops/Torch/Red/Torch_Red.png";
inline const std::string TNT_RED = "resources/tiny_swords/Factions/Goblins/Troops/TNT/Red/TNT_Red.png";

inline const std::string MINIONS_RED = "resources/minions/minionRed.png";
inline const std::string MINIONS_BLUE = "resources/minions/minionBlue.png";

inline const std::string BLUE_SIDE_ARCHER = "resources/tiny_swords/Factions/Knights/Troops/Archer/Blue/Archer_Blue.png";
inline const std::string RED_SIDE_ARCHER = "resources/tiny_swords/Factions/Knights/Troops/Archer/Red/Archer_Red.png";
}  // namespace Troops

namespace Spells {
namespace Garen {
inline const std::string Q = "resources/spells/Lightning.png";
inline const std::string W = "resources/spells/Fire-bomb.png";
inline const std::string E = "resources/spells/spark.png";
inline const std::string R = "resources/spells/Dark-bolt.png";
}  // namespace Garen
}  // namespace Spells
}  // namespace Textures

namespace Window {
constexpr int WIDTH = 1920;
constexpr int HEIGHT = 1080;
inline const std::string TITLE = "League of Pixels";

inline const std::string ICON_PATH = "resources/leagueOfPixelsLogo.png";
}  // namespace Window

namespace Camera {
inline const float MAX_ZOOM = 2.0f;
inline const float MIN_ZOOM = 0.1f;

inline const float SCOLLSPEED = 20.f;
}  // namespace Camera

namespace Minions {
inline const float SPAWN_TIMER = 10.0f;
}
}  // namespace Config
