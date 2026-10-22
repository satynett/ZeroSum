# C++17 Terminal Game Hub

A cross-platform C++17 terminal project for an OOP lab. It uses CMake and the C++ standard library. **The current playable focus is local Math and Texas Hold'em versus bots; networking is not required for this milestone.**

## Build

```sh
cmake -S . -B build
cmake --build build
```

## Play

Launch the menu:

```sh
./build/cpp_game_hub
```

Choose **1** for Math Game vs MathBot or **2** for Texas Hold'em vs PokerBot.

Standalone runners are also built:

```sh
./build/math_local
./build/poker_local
```

On Windows, run the corresponding executable from the build directory (for example, `build\Debug\cpp_game_hub.exe` with Visual Studio, or `build\cpp_game_hub.exe` with MinGW).

## Tests

```sh
ctest --test-dir build --output-on-failure
```

## Current limitations

- Math is a local human-versus-bot terminal mode; the timed modes are simplified for local play.
- Poker is a simplified heads-up Texas Hold'em implementation for local play. It has blinds, hole cards, community cards, hand comparison, and a basic bot, but betting rounds are intentionally simplified.
- LAN server/client/lobby networking is not part of this playable milestone.
