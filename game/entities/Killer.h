//
// Created by Alex on 1/23/2024.
//

#ifndef RAYGAME_KILLER_H
#define RAYGAME_KILLER_H

#include "AStar.h"
#include "Player.h"

namespace game {

    class Killer : public Sprite {
    private:
        AStar astar;
        Map &map;
        Player& player;
        float speed;

    public:
        Killer(std::string path, engine::math::Vec2<float> position, engine::math::Vec2<float> size, Map &map,
               float speed,
               Player &player);
        void update(float deltaTime);

    }; // game
}
#endif //RAYGAME_KILLER_H
