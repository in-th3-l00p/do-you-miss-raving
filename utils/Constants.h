//
// Created by intheloop on 09.01.2024.
//

#ifndef RAYGAME_CONSTANTS_H
#define RAYGAME_CONSTANTS_H

#include "../engine/Entity.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/VideoMode.hpp>
#include "Math.h"

using ull = unsigned long long;

namespace constants {
    // graphics constants
    inline constexpr int DEFAULT_WIDTH = 1280;
    inline constexpr int DEFAULT_HEIGHT = 720;
    inline const std::string DEFAULT_FONT = "resources/fonts/Roboto-Regular.ttf";

    // ingame constants
    inline constexpr float HORIZONTAL_DARKER_MUTLIPLIER = 0.7;

    // testing
    inline constexpr int DEFAULT_TILESIZE = 64;
    inline constexpr float DEFAULT_PLAYER_X = 96;
    inline constexpr float DEFAULT_PLAYER_Y = 96;
    inline constexpr float DEFAULT_PLAYER_DIR_X = 1;
    inline constexpr float DEFAULT_PLAYER_DIR_Y = 0;
    inline constexpr float DEFAULT_PLAYER_RADIUS = 20;
    inline constexpr float DEFAULT_PLAYER_SPEED = 100;
    inline constexpr float DEFAULT_PLAYER_ROTATE_SPEED = DEG_TO_RAD(120);
    inline constexpr float DEFAULT_FOV = DEG_TO_RAD(60);
}

#endif //RAYGAME_CONSTANTS_H
