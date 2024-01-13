//
// Created by Alex on 1/13/2024.
//

#ifndef RAYGAME_ENEMY_H
#define RAYGAME_ENEMY_H

#include <SFML/Graphics/RenderWindow.hpp>
#include "../../engine/Entity.h"
#include "../../utils/Math.h"
#include "../../utils/Constants.h"

namespace game {
    class Enemy : public engine::Entity {
    private:
        engine::math::Vec2<float> position;
        engine::math::Vec2<float> direction;
        float speed;

    public:
        explicit Enemy(
                engine::math::Vec2<float> startPos = { engine::constants::DEFAULT_ENEMY_X, engine::constants::DEFAULT_ENEMY_Y },
                engine::math::Vec2<float> enemyDirection = { engine::constants::DEFAULT_ENEMY_DIR_X, engine::constants::DEFAULT_ENEMY_DIR_Y },
                float enemySpeed = engine::constants::DEFAULT_ENEMY_SPEED
        );
        void render(sf::RenderWindow &window)override;
        void update(float delta) override;

        [[nodiscard]] const engine::math::Vec2<float> &getPosition() const;
    };
} // game

#endif //RAYGAME_ENEMY_H
