//
// Created by intheloop on 11.01.2024.
//

#include "Player.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

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
        window.draw(circle);
        window.draw(line);
    }

    void Player::update(float delta) {
        std::cout << position.x << ' ' << position.y << ' ' << direction.x << ' ' << direction.y << '\n';
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

    Raycaster::Raycaster(Player &player, Map &map):
        player(player), map(map) {
        setZIndex(10);
    }

    void Raycaster::render(sf::RenderWindow &window) {
        int windowWidth = static_cast<int>(window.getSize().x);
        for (int x = 0; x < windowWidth; x++) {
            double cameraX = 2 * x / static_cast<double>(windowWidth) - 1;
            engine::math::Vec2<float> rayDir = player.getDirection() + player.getCameraPlane() * cameraX;

            engine::math::Vec2<int> mapTile = {
                    (int) (std::floor(player.getPosition().x / (float) map.getTileSize())),
                    (int) (std::floor(player.getPosition().y / (float) map.getTileSize()))
            };
            engine::math::Vec2<float> horizontalFirstDist, verticalFirstDist;
            engine::math::Vec2<float> horizontalDeltaDist, verticalDeltaDist;
            bool right = false, down = false;
            if (rayDir.x < 0) {
                horizontalFirstDist.x = (float) mapTile.x * (float) map.getTileSize() - player.getPosition().x;
                horizontalDeltaDist.x = -(float) map.getTileSize();
            } else {
                horizontalFirstDist.x = (float) (mapTile.x + 1) * (float) map.getTileSize() - player.getPosition().x;
                horizontalDeltaDist.x = (float) map.getTileSize();
                right = true;
            }
            float tanAngle = rayDir.y / rayDir.x;
            horizontalFirstDist.y = tanAngle * horizontalFirstDist.x;
            horizontalDeltaDist.y = tanAngle * horizontalDeltaDist.x;

            if (rayDir.y < 0) {
                verticalFirstDist.y = (float) mapTile.y * (float) map.getTileSize() - player.getPosition().y;
                verticalDeltaDist.y = -(float) map.getTileSize();
            } else {
                verticalFirstDist.y = (float) (mapTile.y + 1) * (float) map.getTileSize() - player.getPosition().y;
                verticalDeltaDist.y = (float) map.getTileSize();
                down = true;
            }
            float ctgAngle = 1 / tanAngle;
            verticalFirstDist.x = ctgAngle * verticalFirstDist.y;
            verticalDeltaDist.x = ctgAngle * verticalDeltaDist.y;

            engine::math::Vec2<float> horizontal = player.getPosition() + horizontalFirstDist;
            engine::math::Vec2<int> horizontalTile = mapTile;
            horizontalTile.x += right ? 1 : -1;
            engine::math::Vec2<float> vertical = player.getPosition() + verticalFirstDist;
            engine::math::Vec2<int> verticalTile = mapTile;
            verticalTile.y += down ? 1 : -1;

            double horizontalDistance = std::numeric_limits<double>::infinity();
            double verticalDistance = std::numeric_limits<double>::infinity();

            while (verticalTile.y >= 0 && verticalTile.y < map.getHeight() &&
                    verticalTile.x>=0 && verticalTile.x<map.getWidth() &&
                   map.getTiles()[verticalTile.y][verticalTile.x].empty) {

                vertical += verticalDeltaDist;

                if (down && verticalTile.y < map.getHeight() - 1)
                    verticalTile.y++;
                else if (!down && verticalTile.y > 0)
                    verticalTile.y--;
            }

            verticalDistance = sqrt((player.getPosition().x - vertical.x) * (player.getPosition().x - vertical.x) +
                                    (player.getPosition().y - vertical.y) * (player.getPosition().y - vertical.y));

            while (horizontalTile.x >= 0 && horizontalTile.x < map.getWidth() &&
                    horizontalTile.y >= 0 && horizontalTile.y < map.getHeight() &&
                   map.getTiles()[horizontalTile.y][horizontalTile.x].empty) {

                horizontal += horizontalDeltaDist;

                if (right && horizontalTile.x < map.getWidth() - 1)
                    horizontalTile.x++;
                else if (!right && horizontalTile.x > 0)
                    horizontalTile.x--;
            }

            horizontalDistance = sqrt((player.getPosition().x - horizontal.x) * (player.getPosition().x - horizontal.x) +
                                      (player.getPosition().y - horizontal.y) * (player.getPosition().y - horizontal.y));



            //double horizontalDistance = sqrt( (player.getPosition().x - horizontal.x) * (player.getPosition().x - horizontal.x) + (player.getPosition().y - horizontal.y) * (player.getPosition().y - horizontal.y));
            //double verticalDistance = sqrt( (player.getPosition().x - vertical.x) * (player.getPosition().x - vertical.x) + (player.getPosition().y - vertical.y) * (player.getPosition().y - vertical.y));

            double minDistance = std::min(horizontalDistance, verticalDistance);

                if (!map.getTiles()[horizontalTile.y][horizontalTile.x].empty && minDistance == horizontalDistance) {
                    sf::VertexArray ray(sf::Lines, 2);
                    ray[0].position = {player.getPosition().x, player.getPosition().y};
                    ray[0].color = sf::Color::Green;
                    ray[1].position = {horizontal.x, horizontal.y};
                    ray[1].color = sf::Color::Green;
                    window.draw(ray);
                } else if (!map.getTiles()[verticalTile.y][verticalTile.x].empty && minDistance == verticalDistance) {
                    sf::VertexArray ray(sf::Lines, 2);
                    ray[0].position = {player.getPosition().x, player.getPosition().y};
                    ray[0].color = sf::Color::Green;
                    ray[1].position = {vertical.x, vertical.y};
                    ray[1].color = sf::Color::Green;
                    window.draw(ray);
                }
            }
        }

    void Raycaster::update(float deltaTime) {

    }
} // game