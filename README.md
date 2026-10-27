# C++17 Terminal Game Hub

A C++17 terminal project for an OOP lab. It contains two local games: Matiks against MathBot and Texas Hold'em against PokerBot.

## Build

```sh
cmake -S . -B build
cmake --build build
```

## Play

Launch the game menu:

```sh
./build/cpp_game_hub
```

Choose **1** for Matiks vs MathBot or **2** for Texas Hold'em vs PokerBot.

You can also launch either game directly:

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

- Matiks is a local human-versus-bot terminal game with Sprint, Blitz, and Survival modes.
- Poker is a simplified heads-up Texas Hold'em game against a bot. It includes blinds, hole cards, community cards, hand comparison, and simplified betting rounds.
