cmake_minimum_required(VERSION 3.27)
project(raygame)

add_compile_definitions(SFML_STATIC)
set(CMAKE_CXX_STANDARD 17)

add_executable(raygame main.cpp
        Game.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML/SFML-2.6.1/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

include_directories(C:/SFML/SFML-2.6.1/include)
target_link_libraries(raygame PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
