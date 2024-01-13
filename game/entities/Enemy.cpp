//
// Created by Alex on 1/13/2024.
//

#include <SFML/Graphics.hpp>
#include "Enemy.h"

namespace game {
    Enemy::Enemy(
            engine::math::Vec2<float> startPos,
            engine::math::Vec2<float> enemyDirection,
            float enemySpeed
    ) : position(startPos), direction(enemyDirection), speed(enemySpeed) {
        setZIndex(1);
    }

    void Enemy::render(sf::RenderWindow &window) {
        sf::CircleShape enemyShape(10.0f); // Replace with your preferred representation
        enemyShape.setPosition(position.x , position.y );
        enemyShape.setFillColor(sf::Color::Yellow); // Replace with your preferred color
        window.draw(enemyShape);
    }

    void Enemy::update(float delta) {
        // Implement enemy movement
        position += direction * speed * delta;
    }

    const engine::math::Vec2<float>& Enemy::getPosition() const {
        return position;
    }
} // game