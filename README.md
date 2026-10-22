# C++17 Multiplayer Game Hub

A cross-platform terminal game hub for a C++ OOP lab. Targets Windows (MSVC/MinGW) and macOS (Clang), using CMake and the C++ standard library only.

## Modules
- Math: Sprint, Blitz, Survival, and message-driven MathBot.
- Poker: Texas Hold'em, hand evaluation, betting, and message-driven PokerBot.
- Networking: TCP protocol, lobby, invite codes, server/client, and bot routing.

## Build
```sh
cmake -S . -B build
cmake --build build
```

Run tests with:
```sh
ctest --test-dir build --output-on-failure
```

## Current status
The repository is being assembled in small, reviewable milestones. See docs/architecture.md for contracts and implementation notes.