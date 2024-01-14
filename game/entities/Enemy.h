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
            bool inView;
            engine::math::Vec2<float> position;
            float enemySpeed;
            float direction;
            float distance;
            float number;
    public:
        Enemy(bool inView, engine::math::Vec2<float> position, float enemySpeed, float direction, float distance, float number);
        void render(sf::RenderWindow &window) override;
        void update(float delta) override;

        [[nodiscard]]  const engine::math::Vec2<float> &getPosition() const;

        [[nodiscard]]  bool isInView() const;

        [[nodiscard]]  float getEnemySpeed() const;

        [[nodiscard]]  float getDirection() const;

        [[nodiscard]]  float getDistance() const;

        int getHeight(sf::RenderWindow &window) const;
        int getWidth(sf::RenderWindow &window) const;

        float getNumber() const;

        void setNumber(float number);
    };
} // game

#endif //RAYGAME_ENEMY_H
