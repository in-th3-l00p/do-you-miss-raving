//
// Created by intheloop on 11.01.2024.
//

#include "Player.h"
#include <SFML/Graphics.hpp>
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
        line[1].position = sf::Vector2f(position.x + direction.x * 10, position.y + direction.y * 10);
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            direction = direction * engine::math::getRotationMatrix(rotateSpeed * delta);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            direction = direction * engine::math::getRotationMatrix(-rotateSpeed * delta);
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

    Raycaster::Raycaster(Player &player, Map &map):
        player(player), map(map) {
    }

    void Raycaster::render(sf::RenderWindow &window) {
        for (int x = 0; x < static_cast<int>(window.getSize().x); x++) {
            double cameraX = 2 * x / static_cast<double>(window.getSize().x) - 1;
            engine::math::Vec2<float> rayDir = player.getDirection() + cameraPlane * cameraX;
            engine::math::Vec2<float> mapPos = {
                    static_cast<float>(static_cast<int>(player.getPosition().x)),
                    static_cast<float>(static_cast<int>(player.getPosition().y))
            };
            engine::math::Vec2<float> sideDist;
            engine::math::Vec2<float> deltaDist = {
                    std::abs(1/rayDir.x),
                    std::abs(1/rayDir.y)
            };
            engine::math::Vec2<int> step;
            bool hit = false;
            int side;

            if (rayDir.x < 0) {
                step.x = -1;
                sideDist.x = (player.getPosition().x - mapPos.x) * deltaDist.x;
            } else {
                step.x = 1;
                sideDist.x = (mapPos.x + 1.0f - player.getPosition().x) * deltaDist.x;
            }

            if (rayDir.y < 0) {
                step.y = -1;
                sideDist.y = (player.getPosition().y - mapPos.y) * deltaDist.y;
            } else {
                step.y = 1;
                sideDist.y = (mapPos.y + 1.0f - player.getPosition().y) * deltaDist.y;
            }

            //perform DDA
            while (!hit) {
                //jump to next map square, either in x-direction, or in y-direction
                if (sideDist.x < sideDist.y) {
                    sideDist.x += deltaDist.x;
                    mapPos.x += step.x;
                    side = 0;
                }
                else {
                    sideDist.y += deltaDist.y;
                    mapPos.y += step.y;
                    side = 1;
                }

                //Check if ray has hit a wall
                if (!map.getTiles()[static_cast<int>(mapPos.x)][static_cast<int>(mapPos.y)].empty)
                    hit = true;
            }

            //Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
            double perpWallDist;
            if (side == 0)
                perpWallDist = (mapPos.x - player.getPosition().x + (1 - step.x) / 2) / rayDir.x;
            else
                perpWallDist = (mapPos.y - player.getPosition().y + (1 - step.y) / 2) / rayDir.y;

            //Calculate height of line to draw on screen
            int lineHeight = static_cast<int>(window.getSize().y / perpWallDist);

            //calculate lowest and highest pixel to fill in current stripe
            int drawStart = -lineHeight / 2 + window.getSize().y / 2;
            if(drawStart < 0)
                drawStart = 0;
            int drawEnd = lineHeight / 2 + window.getSize().y / 2;
            if(drawEnd >= window.getSize().y)
                drawEnd = window.getSize().y - 1;

            //choose wall color
            sf::Color color = sf::Color::White;

            //give x and y sides different brightness
            if (side == 1) {
                color.r /= 2;
                color.g /= 2;
                color.b /= 2;
            }

            //draw the pixels of the stripe as a vertical line
            sf::VertexArray line(sf::Lines, 2);
            line[0].position = sf::Vector2f(x, drawStart);
            line[1].position = sf::Vector2f(x, drawEnd);

            line[0].color = color;
            line[1].color = color;

            window.draw(line);

        }
    }

    void Raycaster::update(float deltaTime) {

    }
} // game