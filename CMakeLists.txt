cmake_minimum_required(VERSION 3.16)
project(CppGameHub LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(gamehub_core
  src/math/QuestionGenerator.cpp
  src/poker/Card.cpp
  src/poker/Deck.cpp
  src/poker/HandEvaluator.cpp
  src/Poker.cpp
  src/ui/UI.cpp
  src/Matiks.cpp
)
target_include_directories(gamehub_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(cpp_game_hub main.cpp)
target_link_libraries(cpp_game_hub PRIVATE gamehub_core)
add_executable(math_local tools/math_local.cpp)
target_link_libraries(math_local PRIVATE gamehub_core)
add_executable(poker_local tools/poker_local.cpp)
target_link_libraries(poker_local PRIVATE gamehub_core)

enable_testing()
add_executable(math_tests tests/math_tests.cpp)
target_link_libraries(math_tests PRIVATE gamehub_core)
add_test(NAME math_tests COMMAND math_tests)
add_executable(poker_tests tests/poker_tests.cpp)
target_link_libraries(poker_tests PRIVATE gamehub_core)
add_test(NAME poker_tests COMMAND poker_tests)
