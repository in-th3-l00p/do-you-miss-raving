//
// Created by intheloop on 12.01.2024.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Raycaster.h"

namespace engine {
    engine::Raycaster::Raycaster(game::Player &player, game::Map &map):
            player(player), map(map) {
        setZIndex(10);
        enemies.emplace_back(engine::math::Vec2<float>(200.0f, 200.0f), engine::math::Vec2<float>(1.0f, 0.0f), 10.0f);
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
            sf::Color color;

            double distance;
            if (horizontal.distance < vertical.distance) {
                color = map.getTiles()[horizontal.tile.y][horizontal.tile.x].color;
                color.r = (sf::Uint8) ((float) color.r * engine::constants::HORIZONTAL_DARKER_MUTLIPLIER);
                color.g = (sf::Uint8) ((float) color.g * engine::constants::HORIZONTAL_DARKER_MUTLIPLIER);
                color.b = (sf::Uint8) ((float) color.b * engine::constants::HORIZONTAL_DARKER_MUTLIPLIER);
                distance = horizontal.distance;
            } else {
                color = map.getTiles()[vertical.tile.y][vertical.tile.x].color;
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
            sf::VertexArray line(sf::Lines, 2);
            line[0].position = {(float) x, (float) lineStart};
            line[0].color = color;
            line[1].position = {(float) x, (float) (lineStart + lineHeight)};
            line[1].color = color;
            window.draw(line);
        }
        renderEnemies(window);
    }

    void Raycaster::renderEnemies(sf::RenderWindow& window) {
        for (const auto& enemy : enemies) {

            std::cout << "Enemy Position in World: " << enemy.getPosition().x << ", " << enemy.getPosition().y << std::endl;
            // Calculate enemy position on the screen based on raycasting
            math::Vec2<float> enemyPositionInWorld = enemy.getPosition();
            math::Vec2<float> normalizedDirection = player.getDirection().normalize();
            math::Vec2<float> relativePosition = enemyPositionInWorld - player.getPosition();

            float dotProduct = relativePosition.x * normalizedDirection.x + relativePosition.y * normalizedDirection.y;
            float directionLength = sqrt(player.getDirection().x * player.getDirection().x + player.getDirection().y * player.getDirection().y);
            float relativePositionLength = sqrt(relativePosition.x * relativePosition.x + relativePosition.y * relativePosition.y);
            float cameraX = (enemy.getPosition().x - player.getPosition().x) / player.getDirection().x;
            //float cameraX = dotProduct;
            float projectedEnemyDistance = relativePositionLength * cos(atan2(relativePosition.y, relativePosition.x) - atan2(player.getDirection().y, player.getDirection().x));
            float lineHeight = static_cast<float>(window.getSize().y) / projectedEnemyDistance * map.getTileSize();
            float lineStart = static_cast<float>(window.getSize().y) / 2 - lineHeight / 2;

            std::cout << "ProjectedEnemyDistance: " << projectedEnemyDistance << ", LineHeight: " << lineHeight << ", LineStart: " << lineStart << std::endl;
            std::cout << "relativePositionLength: " << relativePositionLength << ", directionLength: " << directionLength << std::endl;
            std::cout << "cameraX: " << cameraX << std::endl;
            // Example: Draw a simple representation of the enemy
            sf::CircleShape enemyShape(100.0f);
            // Adjust the position based on the enemy's position (use raycasting logic)
            enemyShape.setPosition((float) window.getSize().x / 2 + (float) window.getSize().x * cameraX, lineStart);
            enemyShape.setFillColor(sf::Color::Yellow);
            window.draw(enemyShape);;
        }
    }

    void engine::Raycaster::update(float deltaTime) {
        updateEnemies(deltaTime);
    }

    void Raycaster::updateEnemies(float deltaTime) {
        for (auto& enemy : enemies) {
            enemy.update(deltaTime);
        }
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
        math::Vec2<int> currentTile = mapTile;
        currentTile.x += right ? 1 : -1;
        while (
                currentTile.x >= 0 && currentTile.x < map.getWidth() &&
                currentTile.y >= 0 && currentTile.y < map.getHeight() &&
                map.getTiles()[currentTile.y][currentTile.x].empty
                ) {

            currentCoords += deltaDist;

            if (right && currentTile.x < map.getWidth() - 1)
                currentTile.x++;
            else if (!right && currentTile.x > 0)
                currentTile.x--;
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

        math::Vec2<float> currentCords = player.getPosition() + firstDist;
        math::Vec2<int> currentTile = mapTile;
        currentTile.y += down ? 1 : -1;

        while (
                currentTile.y >= 0 && currentTile.y < map.getHeight() &&
                currentTile.x >= 0 && currentTile.x < map.getWidth() &&
                map.getTiles()[currentTile.y][currentTile.x].empty
                ) {

            currentCords += deltaDist;

            if (down && currentTile.y < map.getHeight() - 1)
                currentTile.y++;
            else if (!down && currentTile.y > 0)
                currentTile.y--;
        }

        Intersection intersection;
        intersection.distance = player.getPosition().getDistance(currentCords);
        intersection.hit = currentCords;
        intersection.tile = currentTile;
        return intersection;
    }
} // engine