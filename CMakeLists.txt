cmake_minimum_required(VERSION 3.26)
project(raygame)

set(CMAKE_CXX_STANDARD 17)

add_executable(raygame main.cpp
        Game.cpp
        Game.h
        config.h)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(raygame PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
