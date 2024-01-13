//
// Created by Alex on 1/13/2024.
//

#include <SFML/Graphics.hpp>
#include "Enemy.h"

namespace game {
    Enemy::Enemy(
            bool inView,
            engine::math::Vec2<float> position,
            float enemySpeed,
            float direction,
            float distance,
            float number
    ) : inView(inView), position(position), enemySpeed(enemySpeed), direction(direction), distance(distance), number(number) {
        setZIndex(1);
    }

    float Enemy::getNumber() const {
        return number;
    }

    void Enemy::setNumber(float number) {
        Enemy::number = number;
    }

    bool Enemy::isInView() const {
        return inView;
    }

    float Enemy::getEnemySpeed() const {
        return enemySpeed;
    }

    float Enemy::getDirection() const {
        return direction;
    }

    float Enemy::getDistance() const {
        return distance;
    }

    int Enemy::getHeight(sf::RenderWindow& window) const
    {
        return round(window.getSize().x / (distance * tan(engine::math::degToRad(0.5f * 90))));
    }

    int Enemy::getWidth(sf::RenderWindow& window) const
    {
        float sprite_height = 10;
        float sprite_width = 10;

        return round(window.getSize().y * sprite_width / (distance * sprite_height * tan(engine::math::degToRad(0.5f * 90))));
    }


    const engine::math::Vec2<float>& Enemy::getPosition() const {
        return position;
    }

    void Enemy::render(sf::RenderWindow& window)
    {
        float sprite_height = 30;
        float sprite_width = 20;

        inView &= window.getSize().y > number + position.y && window.getSize().x > position.x && position.x> -1 * getWidth(window) && number + position.y > -1 * getHeight(window);
    }

    void Enemy::update(float delta) {
        // Implement enemy movement
        position += direction*delta;
    }
} // game