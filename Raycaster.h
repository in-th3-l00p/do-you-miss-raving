//
// Created by intheloop on 12.01.2024.
//

#ifndef RAYGAME_RAYCASTER_H
#define RAYGAME_RAYCASTER_H

#include "Player.h"
#include "Map.h"

namespace engine {
    class Raycaster: public engine::Entity {
    private:
        game::Player& player;
        game::Map& map;

    public:
        Raycaster(game::Player& player, game::Map& map);
        ~Raycaster() {}

        void update(float &deltaTime) {}
        void render(sf::RenderWindow& window) override;

        void update(float deltaTime) override;
    };
} // engine

#endif //RAYGAME_RAYCASTER_H
