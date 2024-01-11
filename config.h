//
// Created by intheloop on 09.01.2024.
//

#ifndef RAYGAME_CONFIG_H
#define RAYGAME_CONFIG_H

#include <SFML/Window/VideoMode.hpp>
#include "Math.h"

using ull = unsigned long long;

namespace config {
    inline constexpr int DEFAULT_WIDTH = 1280;
    inline constexpr int DEFAULT_HEIGHT = 720;

    // testing
    inline constexpr int DEFAULT_TILESIZE = 64;
    inline constexpr float DEFAULT_PLAYER_X = 96;
    inline constexpr float DEFAULT_PLAYER_Y = 96;
    inline constexpr float DEFAULT_PLAYER_DIR_X = 1;
    inline constexpr float DEFAULT_PLAYER_DIR_Y = 0;
    inline constexpr float DEFAULT_PLAYER_RADIUS = 20;
    inline constexpr float DEFAULT_PLAYER_SPEED = 30;
    inline constexpr float DEFAULT_PLAYER_ROTATE_SPEED = DEG_TO_RAD(120);
    inline constexpr float DEFAULT_FOV = DEG_TO_RAD(60);
}

#endif //RAYGAME_CONFIG_H
