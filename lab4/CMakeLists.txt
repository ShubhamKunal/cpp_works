cmake_minimum_required(VERSION 3.15)
project(lab5)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES main.cpp)

add_executable(lab5 ${SOURCES})

add_subdirectory(strategies)
add_subdirectory(game)

target_link_libraries(lab5 strategies game)