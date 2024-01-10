//
// Created by intheloop on 09.01.2024.
//

#ifndef RAYGAME_GAME_H
#define RAYGAME_GAME_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include "Scene.h"

class Game {
private:
    sf::RenderWindow window;
    std::unique_ptr<engine::Scene> scene;

public:
    explicit Game();
    void run();
};


#endif //RAYGAME_GAME_H
