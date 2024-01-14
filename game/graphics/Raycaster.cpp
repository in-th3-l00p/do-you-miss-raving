//
// Created by intheloop on 12.01.2024.
//

#include <SFML/Graphics.hpp>
#include "Raycaster.h"
#include "../../utils/Math.h"
#include <iostream>

namespace engine {
    engine::Raycaster::Raycaster(game::Player &player, game::Map &map):
            player(player), map(map) {
        setZIndex(10);
    }

    void engine::Raycaster::render(sf::RenderWindow &window) {
        int windowWidth = static_cast<int>(window.getSize().x) + 1;
        int windowHeight = static_cast<int>(window.getSize().y);
        sf::RectangleShape floor, ceiling;
        ceiling.setPosition(0, 0);
        ceiling.setSize(sf::Vector2f((float)windowWidth, (float)windowHeight / 2));
        ceiling.setFillColor(sf::Color::Green);
        floor.setPosition(0, (float)windowHeight / 2);
        floor.setSize(sf::Vector2f((float)windowWidth, (float)windowHeight / 2));
        floor.setFillColor(sf::Color::Blue);
        window.draw(floor);
        window.draw(ceiling);

        // drawing the floor and ceiling lighting gradient for distance
        sf::VertexArray floorGradient(sf::Quads, 4);
        sf::VertexArray ceilingGradient(sf::Quads, 4);
        floorGradient[0].position = sf::Vector2f(0, (float)windowHeight / 2);
        floorGradient[1].position = sf::Vector2f((float)windowWidth, (float)windowHeight / 2);
        floorGradient[2].position = sf::Vector2f((float)windowWidth, (float)windowHeight);
        floorGradient[3].position = sf::Vector2f(0, (float)windowHeight);
        floorGradient[0].color = sf::Color(0, 0, 0, 255);
        floorGradient[1].color = sf::Color(0, 0, 0, 255);
        floorGradient[2].color = sf::Color(0, 0, 0, 0);
        floorGradient[3].color = sf::Color(0, 0, 0, 0);

        ceilingGradient[0].position = sf::Vector2f(0, 0);
        ceilingGradient[1].position = sf::Vector2f((float)windowWidth, 0);
        ceilingGradient[2].position = sf::Vector2f((float)windowWidth, (float)windowHeight / 2);
        ceilingGradient[3].position = sf::Vector2f(0, (float)windowHeight / 2);
        ceilingGradient[0].color = sf::Color(0, 0, 0, 0);
        ceilingGradient[1].color = sf::Color(0, 0, 0, 0);
        ceilingGradient[2].color = sf::Color(0, 0, 0, 255);
        ceilingGradient[3].color = sf::Color(0, 0, 0, 255);

        window.draw(floorGradient);
        window.draw(ceilingGradient);

        for (int x = 0; x < windowWidth; x++) {
            float cameraX = 2.f * (float) x / (float) (windowWidth) - 1;
            math::Vec2<float> rayDir = player.getDirection() + player.getCameraPlane() * cameraX;
            math::Vec2<int> mapTile = {
                    (int) (std::floor(player.getPosition().x / (float) map.getTileSize())),
                    (int) (std::floor(player.getPosition().y / (float) map.getTileSize()))
            };
            Intersection horizontal = getHorizontalDistance(cameraX, rayDir, mapTile);
            Intersection vertical = getVerticalDistance(cameraX, rayDir, mapTile);

            const sf::Texture* texture;
            double distance;
            sf::Sprite line;
            int color = 255;
            if (horizontal.distance < vertical.distance) {
                distance = horizontal.distance;
                if (distance > constants::RENDER_DISTANCE)
                    continue;

                texture = &map.getTiles()[horizontal.tile.y][horizontal.tile.x].texture;
                double multiplier = horizontal.hit.y / (double) map.getTileSize() - horizontal.tile.y;
                int hit = std::floor(engine::math::linearInterpolation<double>(multiplier, 0, 1, 0, (double) texture->getSize().y));
                line.setTexture(*texture);
                line.setTextureRect(sf::IntRect(hit, 0, 1, texture->getSize().y));
            } else {
                distance = vertical.distance;
                if (distance > constants::RENDER_DISTANCE)
                    continue;

                texture = &map.getTiles()[vertical.tile.y][vertical.tile.x].texture;
                double multiplier = vertical.hit.x / (double) map.getTileSize() - vertical.tile.x;
                int hit = std::floor(engine::math::linearInterpolation<double>(multiplier, 0, 1, 0, (double) texture->getSize().x));
                line.setTexture(*texture);
                line.setTextureRect(sf::IntRect(hit, 0, 1, texture->getSize().y));

                color = std::floor((float) color * constants::HORIZONTAL_DARKER_MUTLIPLIER);
            }

            // lighting
            color = (int)((float) color * (float) (1 - distance / constants::RENDER_DISTANCE ));
            line.setColor(sf::Color(color, color, color));

            double calculatedAngle =
                    atan2(player.getDirection().y, player.getDirection().x) -
                    atan2(rayDir.y, rayDir.x);
            while (calculatedAngle < 0)
                calculatedAngle += 2 * M_PI;
            while (calculatedAngle > 2 * M_PI)
                calculatedAngle -= 2 * M_PI;

            double lineHeight = (double) windowHeight / (distance * cos(calculatedAngle)) * map.getTileSize();
            double lineStart = (double) windowHeight / 2 - lineHeight / 2;
            line.setScale(1, (float) lineHeight / (float) texture->getSize().y);
            line.setPosition((float) x, (float) lineStart);
            window.draw(line);
        }
    }

    void engine::Raycaster::update(float deltaTime) {

    }

    Intersection Raycaster::getHorizontalDistance(
            double cameraX, math::Vec2<float> rayDir, math::Vec2<int> mapTile
            ) {
        math::Vec2<float> firstDist, deltaDist;
        bool right = false;
        if (rayDir.x < 0) {
            firstDist.x = (float) mapTile.x * (float) map.getTileSize() - player.getPosition().x;
            deltaDist.x = -(float) map.getTileSize();
        } else {
            firstDist.x = (float) (mapTile.x + 1) * (float) map.getTileSize() - player.getPosition().x;
            deltaDist.x = (float) map.getTileSize();
            right = true;
        }
        float tanAngle = rayDir.y / rayDir.x;
        firstDist.y = tanAngle * firstDist.x;
        deltaDist.y = tanAngle * deltaDist.x;

        math::Vec2<float> currentCoords = player.getPosition() + firstDist;
        math::Vec2<int> currentTile = {
                (int)(currentCoords.x / (float) map.getTileSize()) + (right ? 0 : -1),
                (int)(currentCoords.y / (float) map.getTileSize())
        };
        while (
                currentTile.x >= 0 && currentTile.x < map.getWidth() &&
                currentTile.y >= 0 && currentTile.y < map.getHeight() &&
                map.getTiles()[currentTile.y][currentTile.x].empty
                ) {

            currentCoords += deltaDist;
            currentTile = {
                    (int)(currentCoords.x / (float) map.getTileSize()) + (right ? 0 : -1),
                    (int)(currentCoords.y / (float) map.getTileSize())
            };
        }

        Intersection intersection;
        intersection.distance = player.getPosition().getDistance(currentCoords);
        intersection.hit = currentCoords;
        intersection.tile = currentTile;
        return intersection;
    }

    Intersection Raycaster::getVerticalDistance(
            double cameraX, math::Vec2<float> rayDir, math::Vec2<int> mapTile
            ) {
        math::Vec2<float> firstDist, deltaDist;
        bool down = false;
        if (rayDir.y < 0) {
            firstDist.y = (float) mapTile.y * (float) map.getTileSize() - player.getPosition().y;
            deltaDist.y = -(float) map.getTileSize();
        } else {
            firstDist.y = (float) (mapTile.y + 1) * (float) map.getTileSize() - player.getPosition().y;
            deltaDist.y = (float) map.getTileSize();
            down = true;
        }
        float ctgAngle = rayDir.x / rayDir.y;
        firstDist.x = ctgAngle * firstDist.y;
        deltaDist.x = ctgAngle * deltaDist.y;

        math::Vec2<float> currentCoords = player.getPosition() + firstDist;
        math::Vec2<int> currentTile = {
                (int)(currentCoords.x / (float) map.getTileSize()),
                (int)(currentCoords.y / (float) map.getTileSize()) + (down ? 0 : -1)
        };

        while (
                currentTile.y >= 0 && currentTile.y < map.getHeight() &&
                currentTile.x >= 0 && currentTile.x < map.getWidth() &&
                map.getTiles()[currentTile.y][currentTile.x].empty
                ) {

            currentCoords += deltaDist;
            currentTile = {
                    (int)(currentCoords.x / (float) map.getTileSize()),
                    (int)(currentCoords.y / (float) map.getTileSize()) + (down ? 0 : -1)
            };
        }

        Intersection intersection;
        intersection.distance = player.getPosition().getDistance(currentCoords);
        intersection.hit = currentCoords;
        intersection.tile = currentTile;
        return intersection;
    }
} // engine