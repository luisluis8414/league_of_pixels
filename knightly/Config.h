// Config.h
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
        }
    }

    namespace Window {
        constexpr int WIDTH = 1920;
        constexpr int HEIGHT = 1080;
        inline const std::string TITLE = "League of Pixels";
    }
}
