//
// Created by intheloop on 11.01.2024.
//

#ifndef RAYGAME_PLAYER_H
#define RAYGAME_PLAYER_H

#include "Math.h"
#include "Entity.h"
#include "Map.h"

namespace game {

    class Player: public engine::Entity {
    private:
        engine::math::Vec2<float> position;
        engine::math::Vec2<float> direction;
        engine::math::Vec2<float> cameraPlane = { 0, 0.66f };
        float radius, speed, rotateSpeed;

    public:
        explicit Player(
            engine::math::Vec2<float> position = { config::DEFAULT_PLAYER_X, config::DEFAULT_PLAYER_Y },
            engine::math::Vec2<float> direction = { config::DEFAULT_PLAYER_DIR_X, config::DEFAULT_PLAYER_DIR_Y },
            float radius = config::DEFAULT_PLAYER_RADIUS,
            float speed = config::DEFAULT_PLAYER_SPEED,
            float rotateSpeed = config::DEFAULT_PLAYER_ROTATE_SPEED
        );

        void render(sf::RenderWindow& window) override;
        void update(float delta) override;

        [[nodiscard]] const engine::math::Vec2<float> &getPosition() const;
        [[nodiscard]] const engine::math::Vec2<float> &getDirection() const;
        [[nodiscard]] const engine::math::Vec2<float> &getCameraPlane() const;
        [[nodiscard]] float getRadius() const;
        [[nodiscard]] float getSpeed() const;
        [[nodiscard]] float getRotateSpeed() const;
    };

    class Raycaster: public engine::Entity {
    private:
        Player& player;
        Map& map;

    public:
        Raycaster(Player& player, Map& map);
        ~Raycaster() {}

        void update(float &deltaTime) {}
        void render(sf::RenderWindow& window) override;

        void update(float deltaTime) override;
    };
} // game

#endif //RAYGAME_PLAYER_H
