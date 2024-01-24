//
// Created by intheloop on 12.01.2024.
//

#ifndef RAYGAME_RAYCASTER_H
#define RAYGAME_RAYCASTER_H

#include "../entities/Player.h"
#include "../entities/Map.h"

namespace engine {
    struct Intersection {
        double distance;
        math::Vec2<float> hit;
        math::Vec2<int> tile;
    };

    class Raycaster: public engine::Entity {
    private:
        engine::Player& player;
        engine::Map& map;

        // dda for two different sides
        Intersection getHorizontalDistance(
                double cameraX,
                math::Vec2<float> rayDir,
                math::Vec2<int> mapTile
                );
        Intersection getVerticalDistance(
                double cameraX,
                math::Vec2<float> rayDir,
                math::Vec2<int> mapTile
                );

    public:
        Raycaster(
                const std::set<std::unique_ptr<Entity>>& container,
                const std::map<std::string, Entity*>& labels
                );
        ~Raycaster() override = default;
        void render(sf::RenderWindow& window) override;
        void update(float deltaTime) override {};
    };
} // engine

#endif //RAYGAME_RAYCASTER_H
