cmake_minimum_required(VERSION 3.16)
project(CppGameHub LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(gamehub_core src/net/Protocol.cpp)
target_include_directories(gamehub_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(WIN32)
  target_link_libraries(gamehub_core PUBLIC ws2_32)
endif()

enable_testing()
add_executable(protocol_tests tests/protocol_tests.cpp)
target_link_libraries(protocol_tests PRIVATE gamehub_core)
add_test(NAME protocol_tests COMMAND protocol_tests)

# Game and networking targets are added as their modules land.