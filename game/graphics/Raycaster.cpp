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

        for (int x = 0; x < windowWidth; x++) {
            float cameraX = 2.f * (float) x / (float) (windowWidth) - 1;
            math::Vec2<float> rayDir = player.getDirection() + player.getCameraPlane() * cameraX;
            math::Vec2<int> mapTile = {
                    (int) (std::floor(player.getPosition().x / (float) map.getTileSize())),
                    (int) (std::floor(player.getPosition().y / (float) map.getTileSize()))
            };
            Intersection horizontal = getHorizontalDistance(cameraX, rayDir, mapTile);
            Intersection vertical = getVerticalDistance(cameraX, rayDir, mapTile);

            sf::Texture* texture;

            double distance;
            if (horizontal.distance < vertical.distance) {
                const game::Tile& tile = map.getTiles()[horizontal.tile.y][horizontal.tile.x];
                double multiplier = horizontal.hit.y / (double) map.getTileSize() - horizontal.tile.y;
                int hit = std::floor(engine::math::linearInterpolation<double>(multiplier, 0, 1, 0, (double) tile.texture->getWidth()));
                texture = &(*tile.texture)[hit];
                distance = horizontal.distance;
            } else {
                const game::Tile& tile = map.getTiles()[vertical.tile.y][vertical.tile.x];
                double multiplier = vertical.hit.x / (double) map.getTileSize() - vertical.tile.x;
                int hit = std::floor(engine::math::linearInterpolation<double>(multiplier, 0, 1, 0, (double) tile.texture->getWidth()));
                texture = &(*tile.texture)[hit];
                distance = vertical.distance;
            }

            double calculatedAngle =
                    atan2(player.getDirection().y, player.getDirection().x) -
                    atan2(rayDir.y, rayDir.x);
            while (calculatedAngle < 0)
                calculatedAngle += 2 * M_PI;
            while (calculatedAngle > 2 * M_PI)
                calculatedAngle -= 2 * M_PI;

            double lineHeight = (double) windowHeight / (distance * cos(calculatedAngle)) * map.getTileSize();
            double lineStart = (double) windowHeight / 2 - lineHeight / 2;
            sf::Sprite line(*texture);
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