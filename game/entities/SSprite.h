//
// Created by Alex on 1/13/2024.
//

#ifndef RAYGAME_SSPRITE_H
#define RAYGAME_SSPRITE_H

#include <SFML/Graphics/RenderWindow.hpp>
#include "../../engine/Entity.h"
#include "../../utils/Math.h"
#include "../../utils/Constants.h"

namespace game {
    class SSprite : public engine::Entity {
    private:
            bool inView;
            engine::math::Vec2<float> position;
            float enemySpeed;
            float direction;
            float distance;
            float number;
    public:
        explicit SSprite(
            bool inView = true,
            engine::math::Vec2<float> position = {200,200},
            float enemySpeed=10,
            float direction=0,
            float distance=100,
            float number=10
        );
        void render(sf::RenderWindow &window) override;
        void update(float delta) override;

        [[nodiscard]]  const engine::math::Vec2<float> &getPosition() const;

        [[nodiscard]]  bool isInView() const;

        [[nodiscard]]  float getEnemySpeed() const;

        [[nodiscard]]  float getDirection() const;

        [[nodiscard]]  float getDistance() const;

        int getHeight(sf::RenderWindow &window) const;
        int getWidth(sf::RenderWindow &window) const;

        [[nodiscard]] float getNumber() const;

        void setNumber(float number);

        void initTexture();

        void initSprite();

        void drawSprite(const sf::Vector2<short> &i_position, sf::RenderWindow &i_window, const float i_scale_x,
                        const float i_scale_y);

        void setPosition(const engine::math::Vec2<float> &position);
    };
} // game

#endif //RAYGAME_SSPRITE_H
