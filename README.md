# C++17 Terminal Game Hub

A C++17 terminal project for an OOP lab. It contains two local games: Matiks against MathBot and Texas Hold'em against PokerBot.

## Requirements

- A C++17 compiler
- CMake 3.16 or newer

## Build and Run (macOS / Linux)

Run these commands from the project root:

```sh
# Configure the project
cmake -S . -B build

# Compile all targets
cmake --build build

# Start the game menu
./build/cpp_game_hub
```

From the menu, choose **1** for Matiks vs MathBot or **2** for Texas Hold'em vs PokerBot.

### Run one game directly

```sh
./build/math_local
./build/poker_local
```

### Run the tests

```sh
ctest --test-dir build --output-on-failure
```

### If you need a clean rebuild

```sh
rm -rf build
cmake -S . -B build
cmake --build build
```

## Windows

Run the configure and build commands in a terminal with CMake and a C++ compiler available:

```sh
cmake -S . -B build
cmake --build build
```

Then launch the executable produced by your generator. With MinGW, it is typically:

```sh
.\build\cpp_game_hub.exe
```

With Visual Studio, it is typically:

```sh
.\build\Debug\cpp_game_hub.exe
```

Run tests on Windows with:

```sh
ctest --test-dir build --output-on-failure
```

## Current limitations

- Matiks is a local human-versus-bot terminal game with Sprint, Blitz, and Survival modes.
- Poker is a simplified heads-up Texas Hold'em game against a bot. It includes blinds, hole cards, community cards, hand comparison, and simplified betting rounds.
