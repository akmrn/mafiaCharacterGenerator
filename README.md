# Mafia Game

A simple desktop **Mafia game** developed in **C++** using the **SDL3** ecosystem.

The application is designed for playing a local Mafia game with multiple players. Players enter the number of participants, and the program randomly assigns **Mafia** and **Citizen** roles. Each player can then privately view their assigned role before passing the device to the next player.

## Features

* 🎭 Random role assignment
* 👥 Support for **3 to 50 players**
* 🔴 Mafia / Citizen roles
* 🖥️ SDL-based graphical interface
* 🖱️ Mouse input
* 📱 Touch/finger input support
* 🔊 Background music and notification sounds
* 🔇 Mute / unmute audio
* ℹ️ Information screen
* 🔄 Player-by-player role display
* 📐 Logical rendering resolution with letterboxing support

---

# Screenshots

Screenshots can be added to this section once they are available.

Recommended screenshots for the project:

### Main Menu

Show the player-count input screen and the main controls.

```text
screenshots/main-menu.png
```

### Player Message

Show the screen displayed before revealing a player's role.

```text
screenshots/player-message.png
```

### Role Screen

Show the screen where the current player's role is displayed.

```text
screenshots/role-screen.png
```

### Information

Show the application's information/help screen.

```text
screenshots/information.png
```

To display an image in GitHub, place the image inside the `screenshots/` directory and use:

```markdown
![Main Menu](screenshots/main-menu.png)
```

---

# How the Game Works

1. Start the application.
2. Enter the number of players.
3. Press **Next** to start the game.
4. The application randomly generates the roles.
5. Each player views their role privately.
6. The device is passed to the next player.
7. The process continues until every player has received their role.

The current role distribution uses the following rules:

| Number of Players |             Mafia |
| ----------------: | ----------------: |
|               3–5 |                 1 |
|               6–7 |                 2 |
|              8–10 |                 3 |
|               11+ | `playerCount / 3` |

All remaining players are assigned the **Citizen** role.

---

# Project Structure

```text
.
├── main.cpp
├── gui.h
├── gui.cpp
├── windowLoop.h
├── windowLoop.cpp
├── gameLogic.h
├── gameLogic.cpp
├── button.h
├── button.cpp
├── audio.h
├── audio.cpp
├── assets/
└── screenshots/
```

### Main Components

**`main.cpp`**
Application entry point.

**`gui.cpp / gui.h`**
Responsible for creating the SDL window, renderer, textures, fonts, and graphical user interface.

**`windowLoop.cpp / windowLoop.h`**
Contains the main application loop, SDL event handling, input processing, and game-state transitions.

**`gameLogic.cpp / gameLogic.h`**
Handles role generation and randomization.

**`button.cpp / button.h`**
Provides button-related functionality and hit detection.

**`audio.cpp / audio.h`**
Handles background music, notification sounds, mute/unmute functionality, and audio cleanup.

---

# Requirements

The project requires a C++ compiler with support for a modern C++ standard and the SDL libraries used by the application.

The source code uses:

* **SDL3**
* **SDL3_image**
* **SDL3_ttf**
* **SDL3_mixer**

You need both the development headers and the corresponding libraries available to your compiler/linker.

> Make sure the installed SDL versions are compatible with the APIs used by the project.

---

# Installation

## 1. Clone the Repository

```bash
git clone <YOUR_REPOSITORY_URL>
cd <YOUR_REPOSITORY_DIRECTORY>
```

Replace the placeholders with the actual GitHub repository URL and directory name.

## 2. Install SDL Dependencies

Install the development versions of:

```text
SDL3
SDL3_image
SDL3_ttf
SDL3_mixer
```

The exact package names depend on your operating system and package manager.

## 3. Verify the Assets

The application loads graphical, font, and audio resources at runtime.

Make sure the required assets are present in the expected directory structure.

For example:

```text
.
├── mafia
├── assets/
│   ├── images/
│   ├── audio/
│   └── ...
└── ...
```

The exact asset names and paths should match those used by the source code.

## 4. Build the Application

Choose one of the build methods described below.

---

# Building on Linux

After installing the required SDL development libraries, the project can be compiled directly with `g++`.

Example:

```bash
g++ -std=c++17 \
    main.cpp \
    gui.cpp \
    windowLoop.cpp \
    gameLogic.cpp \
    button.cpp \
    audio.cpp \
    -o mafia \
    $(sdl3-config --cflags --libs) \
    -lSDL3_image \
    -lSDL3_ttf \
    -lSDL3_mixer
```

Run the application:

```bash
./mafia
```

If your system uses different SDL package or configuration names, adjust the command accordingly.

---

# CMake

The current project does not include a `CMakeLists.txt`. For a more maintainable cross-platform build, CMake can be added.

A basic `CMakeLists.txt` could look like:

```cmake
cmake_minimum_required(VERSION 3.20)

project(MafiaGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mafia
    main.cpp
    gui.cpp
    windowLoop.cpp
    gameLogic.cpp
    button.cpp
    audio.cpp
)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_mixer REQUIRED)

target_link_libraries(mafia PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    SDL3_mixer::SDL3_mixer
)
```

> The exact imported CMake target names can vary depending on how the SDL libraries were installed. If your SDL installation provides different target names, update the `target_link_libraries()` section accordingly.

## Configure and Build with CMake

Create a build directory:

```bash
cmake -S . -B build
```

Build the project:

```bash
cmake --build build
```

The executable will be generated inside the CMake build directory according to the selected generator and platform.

For a Release build:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

---

# Windows — Visual Studio

## Requirements

Install:

* Visual Studio 2022 or newer
* Desktop development with C++
* SDL3
* SDL3_image
* SDL3_ttf
* SDL3_mixer

## Using CMake

If the project contains a `CMakeLists.txt`, Visual Studio can open the project directly through CMake.

From a Developer PowerShell:

```powershell
cmake -S . -B build
cmake --build build --config Release
```

Alternatively, open the project directory in Visual Studio and allow Visual Studio to configure the CMake project automatically.

## Manual Visual Studio Project

If you are using a traditional `.vcxproj` project, add the following source files:

```text
main.cpp
gui.cpp
windowLoop.cpp
gameLogic.cpp
button.cpp
audio.cpp
```

Configure the SDL include directories under:

```text
Project Properties
    → C/C++
    → General
    → Additional Include Directories
```

Add the SDL library directories under:

```text
Project Properties
    → Linker
    → General
    → Additional Library Directories
```

Then add the required SDL libraries under:

```text
Project Properties
    → Linker
    → Input
    → Additional Dependencies
```

The exact `.lib` names depend on the SDL packages installed on your system.

### DLL Files

At runtime, Windows also needs the corresponding SDL DLL files.

Place the required DLLs either:

* next to the generated executable, or
* in a directory included in the system `PATH`.

---

# Windows — MinGW

With MinGW, make sure the SDL development packages are installed for the same compiler architecture you are using.

For example, if you are compiling a 64-bit application, use 64-bit SDL libraries.

A general compilation command is:

```bash
g++ -std=c++17 ^
    main.cpp ^
    gui.cpp ^
    windowLoop.cpp ^
    gameLogic.cpp ^
    button.cpp ^
    audio.cpp ^
    -I"path\to\SDL3\include" ^
    -I"path\to\SDL3_image\include" ^
    -I"path\to\SDL3_ttf\include" ^
    -I"path\to\SDL3_mixer\include" ^
    -L"path\to\SDL3\lib" ^
    -L"path\to\SDL3_image\lib" ^
    -L"path\to\SDL3_ttf\lib" ^
    -L"path\to\SDL3_mixer\lib" ^
    -lSDL3 ^
    -lSDL3_image ^
    -lSDL3_ttf ^
    -lSDL3_mixer ^
    -o mafia.exe
```

Replace the paths with the locations of your SDL installations.

Run:

```powershell
.\mafia.exe
```

Make sure the required SDL DLL files are accessible to the executable.

---

# Assets

The application loads graphical and audio resources at runtime.

Make sure the required assets are available in the expected location relative to the executable.

If the program starts but fails to load textures, fonts, or audio, check that:

1. The asset files exist.
2. Their filenames are correct.
3. Their paths match the paths expected by the source code.
4. The working directory is correct.
5. The application has permission to read the files.

For development, it is recommended to run the executable from the project's expected working directory.

---

# Input

The application supports both:

* Mouse input
* Touch/finger input

The graphical interface uses a logical rendering resolution, allowing the UI to adapt to different window sizes.

---

# Audio

The audio system provides:

* Background audio
* Notification sounds
* Mute / unmute functionality
* Audio resource cleanup

Audio is managed through **SDL3_mixer**.

If audio is not working, verify that:

* SDL3_mixer is installed correctly.
* The required audio files exist.
* The audio files can be loaded by SDL3_mixer.
* The required runtime libraries are available.

---

# Game States

The application uses several stages to control the game flow:

```text
Player Count
     │
     ▼
Show Player Message
     │
     ▼
Show Role
     │
     ├── Next Player ──► Show Player Message
     │
     ▼
Finished
```

After all players have received their roles, the application returns to the player-count screen so a new game can be started.

---

# Troubleshooting

## `SDL3.h: No such file or directory`

The compiler cannot find the SDL3 headers.

Check that SDL3 development files are installed and that the SDL include directory is included in the compiler's include path.

For example:

```bash
-I/path/to/SDL3/include
```

---

## SDL library cannot be found during linking

Errors such as:

```text
cannot find -lSDL3
```

usually indicate that the linker cannot find the SDL library.

Check your library path:

```bash
-L/path/to/SDL3/lib
```

Also verify that the correct architecture is being used.

For example, don't mix 32-bit SDL libraries with a 64-bit compiler.

---

## DLL missing on Windows

If Windows reports an error such as:

```text
The code execution cannot proceed because SDL3.dll was not found.
```

copy the required SDL DLL files next to the executable or add their directory to `PATH`.

The same applies to the SDL3_image, SDL3_ttf, and SDL3_mixer dependencies.

---

## Application starts but assets are missing

If the window opens but images, fonts, or audio cannot be loaded, the most likely cause is an incorrect working directory or asset path.

Try running the application from the project directory:

```bash
./mafia
```

or, on Windows:

```powershell
.\mafia.exe
```

Then verify the asset paths used by the source code.

---

## Audio does not work

Check:

1. SDL3_mixer is installed.
2. The mixer library is correctly linked.
3. Required audio files exist.
4. The audio files are in the expected location.
5. The corresponding SDL runtime libraries are available.

If audio initialization fails, check the application's error output for more information.

---

## Buttons do not respond correctly after resizing the window

The application uses logical rendering and letterboxing.

Different window aspect ratios can expose coordinate-mapping issues, particularly around UI elements that use manually calculated physical coordinates.

If this occurs, try using the application's default window size first.

This is also an area that can be improved by consistently converting all pointer events to logical coordinates before performing hit tests.

---

## `std::stoi` or player-count input error

The application expects a valid player count between:

```text
3
```

and:

```text
50
```

Enter only numeric characters and make sure the resulting number is within the supported range.

---

## CMake cannot find SDL

If CMake reports that it cannot find one of the SDL packages, make sure the development packages and their CMake configuration files are installed.

You may need to specify the installation prefix manually:

```bash
cmake -S . -B build \
    -DCMAKE_PREFIX_PATH=/path/to/SDL/installation
```

On Windows:

```powershell
cmake -S . -B build `
    -DCMAKE_PREFIX_PATH="C:\path\to\SDL"
```

The exact path depends on how SDL was installed.

---

# Randomization

Roles are generated dynamically for each game and shuffled using C++'s standard random facilities.

The generated role list contains:

```text
Mafia
Mafia
...
Citizen
Citizen
...
```

The complete list is then shuffled before being assigned to players.

---

# Development Notes

This project is intended primarily as a small SDL3/C++ game project and learning-oriented application.

Potential areas for improvement include:

* More robust resource management using RAII
* A more explicit state-machine architecture
* Centralized asset loading
* Improved input coordinate handling for letterboxed windows
* More structured role types instead of strings
* Better error handling
* Unit tests for the game-logic layer
* CMake-based build automation
* Cross-platform packaging

---

# Future Improvements

Possible future additions include:

* More Mafia roles
* Custom role distribution
* Game configuration menu
* Player names
* Additional sound effects
* Animations and transitions
* Better responsive UI
* CMake build configuration
* Automated tests
* Cross-platform packaging

---

# License

Add your preferred license here.

For example:

```text
MIT License
```

If no license has been selected yet, the project is currently available without an explicit open-source license.
