cmake_minimum_required(VERSION 3.26)
project(raygame)

add_compile_definitions(SFML_STATIC)
set(CMAKE_CXX_STANDARD 17)

add_executable(raygame main.cpp
        Game.cpp
        Scene.cpp
        Scene.h
        Entity.cpp
        Entity.h
        Map.cpp
        Map.h
        Player.cpp
        Player.h
        Math.cpp
        Math.h)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(raygame PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
