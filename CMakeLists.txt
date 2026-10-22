cmake_minimum_required(VERSION 3.16)
project(CppGameHub LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(gamehub_core
  src/net/Protocol.cpp
  src/net/Socket.cpp
  src/net/InviteCode.cpp
  src/math/QuestionGenerator.cpp
  src/math/MathGame.cpp
  src/math/MathBot.cpp
  src/poker/Card.cpp
  src/poker/Deck.cpp
  src/poker/HandEvaluator.cpp
  src/poker/PokerBot.cpp
  src/local/LocalGames.cpp
  src/local/MathArena.cpp
)
target_include_directories(gamehub_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
if(WIN32)
  target_link_libraries(gamehub_core PUBLIC ws2_32)
endif()

add_executable(cpp_game_hub main.cpp)
target_link_libraries(cpp_game_hub PRIVATE gamehub_core)
add_executable(math_local tools/math_local.cpp)
target_link_libraries(math_local PRIVATE gamehub_core)
add_executable(poker_local tools/poker_local.cpp)
target_link_libraries(poker_local PRIVATE gamehub_core)

enable_testing()
add_executable(protocol_tests tests/protocol_tests.cpp)
target_link_libraries(protocol_tests PRIVATE gamehub_core)
add_test(NAME protocol_tests COMMAND protocol_tests)
add_executable(math_tests tests/math_tests.cpp)
target_link_libraries(math_tests PRIVATE gamehub_core)
add_test(NAME math_tests COMMAND math_tests)
add_executable(poker_tests tests/poker_tests.cpp)
target_link_libraries(poker_tests PRIVATE gamehub_core)
add_test(NAME poker_tests COMMAND poker_tests)
add_executable(invite_tests tests/invite_tests.cpp)
target_link_libraries(invite_tests PRIVATE gamehub_core)
add_test(NAME invite_tests COMMAND invite_tests)
