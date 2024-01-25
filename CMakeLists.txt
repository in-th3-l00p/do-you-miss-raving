cmake_minimum_required(VERSION 3.26)
project(raygame)

add_compile_definitions(SFML_STATIC)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

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
        game/entities/SpriteEntity.cpp
        game/entities/SpriteEntity.h

        # imgui
        lib/imgui/imgui.cpp
        lib/imgui/imgui_draw.cpp
        lib/imgui/imgui_widgets.cpp
        lib/imgui/imgui_demo.cpp
        lib/imgui-sfml/imgui-SFML.cpp
        lib/imgui/imgui_tables.cpp
)

# luci
if (WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
    set(SFML_DIR C:/SFML/SFML-2.6.1/lib/cmake/SFML)
    include_directories(C:/SFML/SFML-2.6.1/include)
endif ()

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(raygame PRIVATE sfml-system sfml-graphics sfml-window ${OPENGL_LIBRARIES})

target_include_directories(raygame PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/imgui
        ${CMAKE_SOURCE_DIR}/lib/imgui-sfml
)

# copying everything inside of the /resources directory to the build directory
# so the resources can be accessed inside of the executable
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})