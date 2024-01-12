//
// Created by intheloop on 11.01.2024.
//

#ifndef RAYGAME_PLAYER_H
#define RAYGAME_PLAYER_H

#include "../../utils/Math.h"
#include "../../engine/Entity.h"
#include "Map.h"
#include "../../utils/Constants.h"

namespace game {

    class Player: public utils::Entity {
    private:
        utils::math::Vec2<float> position;
        utils::math::Vec2<float> direction;
        utils::math::Vec2<float> cameraPlane = {0, 0.66f };
        float radius, speed, rotateSpeed;

    public:
        explicit Player(
                utils::math::Vec2<float> position = {utils::constants::DEFAULT_PLAYER_X, utils::constants::DEFAULT_PLAYER_Y },
                utils::math::Vec2<float> direction = {utils::constants::DEFAULT_PLAYER_DIR_X, utils::constants::DEFAULT_PLAYER_DIR_Y },
                float radius = utils::constants::DEFAULT_PLAYER_RADIUS,
                float speed = utils::constants::DEFAULT_PLAYER_SPEED,
                float rotateSpeed = utils::constants::DEFAULT_PLAYER_ROTATE_SPEED
        );

        void render(sf::RenderWindow& window) override;
        void update(float delta) override;

        [[nodiscard]] const utils::math::Vec2<float> &getPosition() const;
        [[nodiscard]] const utils::math::Vec2<float> &getDirection() const;
        [[nodiscard]] const utils::math::Vec2<float> &getCameraPlane() const;
        [[nodiscard]] float getRadius() const;
        [[nodiscard]] float getSpeed() const;
        [[nodiscard]] float getRotateSpeed() const;
    };

} // game

#endif //RAYGAME_PLAYER_H
