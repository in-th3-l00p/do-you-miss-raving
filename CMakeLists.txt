cmake_minimum_required(VERSION 3.26)
project(raygame)

add_compile_definitions(SFML_STATIC)
set(CMAKE_CXX_STANDARD 17)

add_executable(raygame main.cpp
        engine/Game.cpp
        engine/Scene.cpp
        engine/Scene.h
        engine/Entity.cpp
        engine/Entity.h
        game/entities/Map.cpp
        game/entities/Map.h
        game/entities/Player.cpp
        game/entities/Player.h
        utils/Math.cpp
        utils/Math.h
        game/graphics/Raycaster.cpp
        game/graphics/Raycaster.h
        game/graphics/UserInterface.cpp
        game/graphics/UserInterface.h
        utils/Paths.h
        game/entities/SSprite.cpp
        game/entities/SSprite.h)

# luci
if (WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
    set(SFML_DIR C:/SFML/SFML-2.6.1/lib/cmake/SFML)
    include_directories(C:/SFML/SFML-2.6.1/include)
endif ()

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(raygame PRIVATE sfml-system sfml-graphics sfml-window)

# copying everything inside of the /resources directory to the build directory
# so the resources can be accessed inside of the executable
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})