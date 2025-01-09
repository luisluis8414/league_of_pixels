workspace "LeagueOfPixels"
   architecture "x64" 
   configurations { "Debug", "Release" }

project "LeagueOfPixels"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   
   targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}"
   objdir "obj/%{cfg.platform}/%{cfg.buildcfg}"

   files { "src/**.h", "src/**.cpp" }

   includedirs { "include" }

   libdirs { "lib" }
   links { 
      "sfml-graphics-d", 
      "sfml-window-d", 
      "sfml-system-d", 
   }
   
   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
      postbuildcommands {
         "{COPY} vendor/SFML-3.0.0/bin/sfml-graphics-d-3.dll %{cfg.targetdir}",
         "{COPY} vendor/SFML-3.0.0/bin/sfml-window-d-3.dll %{cfg.targetdir}",
         "{COPY} vendor/SFML-3.0.0/bin/sfml-system-d-3.dll %{cfg.targetdir}",
        --  "{COPY} vendor/SFML-3.0.0/bin/sfml-audio-d-2.dll %{cfg.targetdir}",
        --  "{COPY} vendor/SFML-3.0.0/bin/sfml-network-d-2.dll %{cfg.targetdir}"
      }

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      postbuildcommands {
         "{COPY} vendor/SFML-3.0.0/bin/sfml-graphics-3.dll %{cfg.targetdir}",
         "{COPY} vendor/SFML-3.0.0/bin/sfml-window-3.dll %{cfg.targetdir}",
         "{COPY} vendor/SFML-3.0.0/bin/sfml-system-3.dll %{cfg.targetdir}",
        --  "{COPY} vendor/SFML-3.0.0/bin/sfml-audio-2.dll %{cfg.targetdir}",
        --  "{COPY} vendor/SFML-3.0.0/bin/sfml-network-2.dll %{cfg.targetdir}"
      }

   prebuildcommands {
      "mkdir -p bin/%{cfg.buildcfg}",
      "mkdir -p obj/%{cfg.buildcfg}"
   }
