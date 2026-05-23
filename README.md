# League of Pixels

> **Work Halted**
> This project is not finished and work is currently halted.

![Demo](assets/demo.gif)

## Build Instructions

### Overview

This project uses Premake to generate Visual Studio project files and SFML for rendering. The repository includes Premake and SFML 3.0.0 binaries configured for Visual Studio on Windows.

Visual Studio is the supported compiler on Windows. MinGW needs a different SFML build and is not wired up here.

### Prerequisites

- Visual Studio 2022
- The **Desktop development with C++** workload
- Windows SDK, installed through the Visual Studio installer

### Build on Windows

From the repository root, run:

```bat
.\scripts\build.bat
```

This generates `LeagueOfPixels.sln` with Premake and builds the default `Debug` configuration with MSBuild.

To build `Release` instead:

```bat
.\scripts\build.bat Release
```

### Run on Windows

From the repository root, run:

```bat
.\scripts\run.bat
```

To run `Release` instead:

```bat
.\scripts\run.bat Release
```

The run script starts the executable from `bin\<configuration>` so the game's relative `resources/...` paths resolve correctly.

### Output

Build outputs are written to:

```text
bin\Debug\LeagueOfPixels.exe
bin\Release\LeagueOfPixels.exe
```

Premake's post-build step copies the required resources and SFML DLLs into the output directory.

Debug DLLs:

- `sfml-graphics-d-3.dll`
- `sfml-window-d-3.dll`
- `sfml-system-d-3.dll`

Release DLLs:

- `sfml-graphics-3.dll`
- `sfml-window-3.dll`
- `sfml-system-3.dll`

### Opening in Visual Studio

If you prefer building manually:

1. Run `.\vendor\premake5\premake5.exe vs2022`.
2. Open `LeagueOfPixels.sln`.
3. Select `Debug|x64` or `Release|x64`.
4. Build the project.

### MinGW Note

The vendored SFML libraries are Visual Studio libraries. To use MinGW, download an SFML 3.0.0 MinGW package, replace the `include` and `lib` files with compatible ones, then generate Makefiles with:

```bat
.\vendor\premake5\premake5.exe gmake2
```
