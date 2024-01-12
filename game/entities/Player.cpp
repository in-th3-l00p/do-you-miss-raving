//
// Created by intheloop on 11.01.2024.
//

#include "Player.h"
#include <SFML/Graphics.hpp>

namespace game {
    Player::Player(
            engine::math::Vec2<float> position,
            engine::math::Vec2<float> direction,
            float radius,
            float speed,
            float rotateSpeed
    ): position(position), direction(direction), radius(radius), speed(speed), rotateSpeed(rotateSpeed) {
        setZIndex(1);
    }

    void Player::render(sf::RenderWindow &window) {
        sf::CircleShape circle(radius);
        circle.setPosition(position.x - radius, position.y - radius);
        circle.setFillColor(sf::Color::Red);
        sf::VertexArray line(sf::Lines, 2);
        line[0].position = sf::Vector2f(position.x, position.y);
        line[1].position = sf::Vector2f(position.x + direction.x * 30, position.y + direction.y * 30);
        line[0].color = sf::Color::Red;
        line[1].color = sf::Color::Red;
//        window.draw(circle);
//        window.draw(line);
    }

    void Player::update(float delta) {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            position += direction * speed * delta;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            position -= direction * speed * delta; // i show speed

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            engine::math::Mat2<float> rotMat = engine::math::getRotationMatrix(rotateSpeed * delta);
            direction = direction * rotMat;
            cameraPlane = cameraPlane * rotMat;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            engine::math::Mat2<float> rotMat = engine::math::getRotationMatrix(-rotateSpeed * delta);
            direction = direction * rotMat;
            cameraPlane = cameraPlane * rotMat;
        }
    }

    const engine::math::Vec2<float> &Player::getPosition() const {
        return position;
    }

    const engine::math::Vec2<float> &Player::getDirection() const {
        return direction;
    }

    float Player::getRadius() const {
        return radius;
    }

    float Player::getSpeed() const {
        return speed;
    }

    float Player::getRotateSpeed() const {
        return rotateSpeed;
    }

    const engine::math::Vec2<float> &Player::getCameraPlane() const {
        return cameraPlane;
    }
} // game