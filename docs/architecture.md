# Architecture

The project is a local terminal game hub. Matiks and Texas Hold'em run in the same process.

## Main files

- `main.cpp` starts the game hub.
- `src/ui/UI.cpp` contains the main menu, shared input helpers, and terminal rendering.
- `src/Matiks.cpp` contains Matiks gameplay, scoring, timed modes, and MathBot decisions.
- `src/Poker.cpp` contains Texas Hold'em gameplay, betting, showdown, and PokerBot decisions.
- `src/math/QuestionGenerator.cpp` generates math questions.
- `src/poker/` contains card, deck, and hand-evaluation helpers.

## Build and test

Configure and build with CMake, then run the local test suite:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```
