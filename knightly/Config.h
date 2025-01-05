#pragma once
#include <string>

namespace Config {
    namespace Fonts {
        inline const std::string ARIAL = "resources/fonts/arial.ttf";
    }

    namespace Textures {
        namespace Buildings {
            inline const std::string CASTLE_RED = "resources/tiny_swords/Factions/Knights/Buildings/Castle/Castle_Red.png";
            inline const std::string CASTLE_BLUE = "resources/tiny_swords/Factions/Knights/Buildings/Castle/Castle_Blue.png";
        }

        namespace Troops {
            inline const std::string TORCH_RED = "resources/tiny_swords/Factions/Goblins/Troops/Torch/Red/Torch_Red.png";
            inline const std::string TNT_RED = "resources/tiny_swords/Factions/Goblins/Troops/TNT/Red/TNT_Red.png";

            inline const std::string MINIONS_RED = "resources/minions/minionRed.png";
            inline const std::string MINIONS_BLUE = "resources/minions/minionBlue.png";
        }
    }

    namespace Window {
        constexpr int WIDTH = 1920;
        constexpr int HEIGHT = 1080;
        inline const std::string TITLE = "League of Pixels";

        inline const std::string ICON_PATH = "resources/leagueOfPixelsLogo.png";
    }

    namespace Camera {
        inline const float MAX_ZOOM = 2.0f;
        inline const float MIN_ZOOM = 0.1f;
    }

    namespace Minions {
        inline const float SPAWN_TIMER = 10.0f;
    }
}
