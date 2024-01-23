//
// Created by intheloop on 11.01.2024.
//

#ifndef RAYGAME_PLAYER_H
#define RAYGAME_PLAYER_H

#include "../../utils/Math.h"
#include "../../engine/Entity.h"
#include "../../utils/Constants.h"
#include "Map.h"

namespace game {
    class Player: public engine::Entity {
    private:
        engine::math::Vec2<float> position;
        engine::math::Vec2<float> direction;
        engine::math::Vec2<float> cameraPlane = {0, 0.66f };
        float radius, speed, rotateSpeed;
        float stamina;
        const float maxStamina;
        const float staminaRegen;
        bool isRunning;
        Map& map;
    public:
        explicit Player(
                game::Map& map,
                engine::math::Vec2<float> position = {engine::constants::DEFAULT_PLAYER_X, engine::constants::DEFAULT_PLAYER_Y },
                engine::math::Vec2<float> direction = {engine::constants::DEFAULT_PLAYER_DIR_X, engine::constants::DEFAULT_PLAYER_DIR_Y },
                float radius = engine::constants::DEFAULT_PLAYER_RADIUS,
                float speed = engine::constants::DEFAULT_PLAYER_SPEED,
                float rotateSpeed = engine::constants::DEFAULT_PLAYER_ROTATE_SPEED
        );

        void render(sf::RenderWindow& window) override;
        void update(float delta) override;

        [[nodiscard]] const engine::math::Vec2<float> &getPosition() const;
        [[nodiscard]] const engine::math::Vec2<float> &getDirection() const;
        [[nodiscard]] const engine::math::Vec2<float> &getCameraPlane() const;
        [[nodiscard]] float getRadius() const;
        [[nodiscard]] float getSpeed() const;
        [[nodiscard]] float getRotateSpeed() const;
        float getStamina() const;

        void renderStaminaBar(sf::RenderWindow &window);

        ull PlayerGetTileSize() const;

        ull PlayerGetWidth() const;

        ull PlayerGetHeight() const;

        Tile &PlayergetTile(int i, int i1);
    };

} // game

#endif //RAYGAME_PLAYER_H
