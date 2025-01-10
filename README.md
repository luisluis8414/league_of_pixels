# League of Pixels

## Build Instructions

### Overview

This project uses **Premake** for generating build files and **SFML** for rendering. The build process has been streamlined using a batch script located in the `scripts` folder. Currently, **Visual Studio** is the only supported compiler due to SFML compatibility issues with MinGW. Using MinGW requires downloading and configuring a different version of SFML, which is not supported at this time.

### Steps to Build

#### 1. Prerequisites

- **Premake**: The project includes a local version of Premake located in `vendor/premake5`.
- **SFML**: The project uses SFML 3.0.0, provided in the `vendor/SFML-3.0.0` directory. This version is pre-configured for Visual Studio compilers.

#### 2. Generate Build Files

Run the batch script to generate the Visual Studio solution:

```bash
.\scripts\build.bat
```

### This script

1. **Navigates to the `vendor/premake5` directory**  
   The script changes to the directory containing `premake5.exe`.

2. **Runs `premake5.exe vs2022`**  
   This generates a Visual Studio 2022 solution file.

3. **Returns to the root directory**  
   Once the solution is generated, the script navigates back to the root directory and confirms the process.

The generated solution file can then be opened in Visual Studio.

---

### 3. Build the Project in Visual Studio

1. Open the generated `.sln` file in Visual Studio.
2. Select the desired build configuration (`Debug` or `Release`).
3. Build the project.

---

### 4. Run the Application

After building the project, ensure the required SFML DLLs are present in the `bin/<platform>/<configuration>` directory:

- **For Debug**:
  - `sfml-graphics-d-2.dll`
  - `sfml-window-d-2.dll`
  - `sfml-system-d-2.dll`

- **For Release**:
  - `sfml-graphics-2.dll`
  - `sfml-window-2.dll`
  - `sfml-system-2.dll`

These DLLs are automatically copied during the build process via `postbuildcommands` in the `premake5.lua` file. If any DLLs are missing, verify the `postbuildcommands` section or manually copy the required DLLs.

---

### 5. Limitations

- **Only Visual Studio is Supported**  
  The provided version of SFML is configured for Visual Studio compilers. MinGW is not supported due to the need for a MinGW-compatible version of SFML.

- **Manual SFML Configuration for MinGW**  
  If MinGW support is required, follow these steps:
  1. Download a MinGW-compatible version of SFML 3.0.0 from the [SFML website](https://www.sfml-dev.org/download/sfml/3.0.0/).
  2. Place the downloaded SFML library files into the `lib` directory of the project.
  3. Place the downloaded SFML include files into the `include` directory of the project.
  5. Reconfigure the project by running the build script (`.\scripts\build.bat`) to regenerate the solution files or run:

  ```bash
  .\vendor\premake5\premake5.exe gmake2  
  ```

