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
        sf::Image background;
        background.create(constants::DEFAULT_WIDTH / 2, constants::DEFAULT_HEIGHT / 2, sf::Color::Red);
        for (int y = 0; y < background.getSize().y; y++) {
            engine::math::Vec2<float> rayDir0(
                    player.getDirection().x - player.getCameraPlane().x,
                    player.getDirection().y - player.getCameraPlane().y
            );
            engine::math::Vec2<float> rayDir1(
                    player.getDirection().x + player.getCameraPlane().x,
                    player.getDirection().y + player.getCameraPlane().y
            );

            int p = y - background.getSize().y / 2;
            float posZ = 0.5f * background.getSize().y;
            float rowDistance = posZ / p;

            engine::math::Vec2<float> floorStep = {
                    rowDistance * (rayDir1.x - rayDir0.x) / background.getSize().x,
                    rowDistance * (rayDir1.y - rayDir0.y) / background.getSize().x
            };
            engine::math::Vec2<float> floor = {
                    player.getPosition().x / (float) map.getTileSize() + rowDistance * rayDir0.x,
                    player.getPosition().y / (float) map.getTileSize() + rowDistance * rayDir0.y
            };

            for (int x = 0; x < background.getSize().x; x++) {
                int cellX = (int) floor.x;
                int cellY = (int) floor.y;
                if (cellX < 0)
                    cellX = 0;
                if (cellX >= map.getWidth())
                    cellX = map.getWidth() - 1;
                if (cellY < 0)
                    cellY = 0;
                if (cellY >= map.getHeight())
                    cellY = map.getHeight() - 1;

                auto& tile = map.getTiles()[cellY][cellX];
                int tx = (int) (tile.image.getSize().x * (floor.x - cellX)) & (tile.image.getSize().x - 1);
                int ty = (int) (tile.image.getSize().y * (floor.y - cellY)) & (tile.image.getSize().y - 1);

                floor.x += floorStep.x;
                floor.y += floorStep.y;

                if (tx < 0)
                    tx = 0;
                if (ty < 0)
                    ty = 0;
                if (tx >= map.getTileSize())
                    tx = map.getTileSize() - 1;
                if (ty >= map.getTileSize())
                    ty = map.getTileSize() - 1;

                sf::Color floorColor = tile.image.getPixel(tx, ty);
                floorColor.r = std::floor((float) floorColor.r * ((float) y / (float) (background.getSize().y - 1)));
                floorColor.g = std::floor((float) floorColor.g * ((float) y / (float) (background.getSize().y - 1)));
                floorColor.b = std::floor((float) floorColor.b * ((float) y / (float) (background.getSize().y - 1)));
                background.setPixel(x, y, floorColor);

                sf::Color ceilingColor = tile.image.getPixel(tx, ty);
                ceilingColor.r = std::floor((float) ceilingColor.r * ((float) y / (float) (background.getSize().y - 1)));
                ceilingColor.g = std::floor((float) ceilingColor.g * ((float) y / (float) (background.getSize().y - 1)));
                ceilingColor.b = std::floor((float) ceilingColor.b * ((float) y / (float) (background.getSize().y - 1)));
                background.setPixel(x, background.getSize().y - y - 1, ceilingColor);
            }
        }

        sf::Texture backgroundTexture;
        backgroundTexture.loadFromImage(background);
        sf::Sprite backgroundSprite(backgroundTexture);
        backgroundSprite.setPosition(0, 0);
        backgroundSprite.setScale(
                (float) windowWidth / background.getSize().x,
                (float) windowHeight / background.getSize().y
                );
        window.draw(backgroundSprite);

        // z buffer, first what should be rendered, second the distance
        std::vector<std::pair<sf::Sprite*, double>> zBuffer;

        // adding lines
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

            // to do implement it in the stack
            auto* line = new sf::Sprite();
            int color = 255;
            if (horizontal.distance < vertical.distance) {
                distance = horizontal.distance;
                texture = &map.getTiles()[horizontal.tile.y][horizontal.tile.x].texture;
                double multiplier = horizontal.hit.y / (double) map.getTileSize() - horizontal.tile.y;
                int hit = std::floor(engine::math::linearInterpolation<double>(multiplier, 0, 1, 0, (double) texture->getSize().y));
                line->setTexture(*texture);
                line->setTextureRect(sf::IntRect(hit, 0, 1, texture->getSize().y));
            } else {
                distance = vertical.distance;
                texture = &map.getTiles()[vertical.tile.y][vertical.tile.x].texture;
                double multiplier = vertical.hit.x / (double) map.getTileSize() - vertical.tile.x;
                int hit = std::floor(engine::math::linearInterpolation<double>(multiplier, 0, 1, 0, (double) texture->getSize().x));
                line->setTexture(*texture);
                line->setTextureRect(sf::IntRect(hit, 0, 1, texture->getSize().y));

                color = std::floor((float) color * constants::HORIZONTAL_DARKER_MUTLIPLIER);
            }

            // lighting
            color = (int)((float) color * (float) (1 - std::min((float) distance, constants::RENDER_DISTANCE / 2) / constants::RENDER_DISTANCE ));
            line->setColor(sf::Color(color, color, color));

            double calculatedAngle =
                    atan2(player.getDirection().y, player.getDirection().x) -
                    atan2(rayDir.y, rayDir.x);
            while (calculatedAngle < 0)
                calculatedAngle += 2 * M_PI;
            while (calculatedAngle > 2 * M_PI)
                calculatedAngle -= 2 * M_PI;

            double lineHeight = (double) windowHeight / (distance * cos(calculatedAngle)) * map.getTileSize();
            double lineStart = (double) windowHeight / 2 - lineHeight / 2;
            line->setScale(1, (float) lineHeight / (float) texture->getSize().y);
            line->setPosition((float) x, (float) lineStart);
            zBuffer.emplace_back(line, distance);
        }

        // putting the sprites in the z buffer
        for (auto sprite: map.getSprites()) {
            double spriteDistance = player.getPosition().getDistance(sprite.position);
            if (spriteDistance > constants::RENDER_DISTANCE)
                continue;

            auto* spriteInstance = new sf::Sprite(sprite.sprite);
            float spriteX = sprite.position.x - player.getPosition().x + sprite.size.x / 2;
            float spriteY = sprite.position.y - player.getPosition().y + sprite.size.y / 2;

            float invDet = 1.0f / (player.getCameraPlane().x * player.getDirection().y - player.getDirection().x * player.getCameraPlane().y);
            float transformX = invDet * (player.getDirection().y * spriteX - player.getDirection().x * spriteY);
            float transformY = invDet * (-player.getCameraPlane().y * spriteX + player.getCameraPlane().x * spriteY);

            float screenX = (float) windowWidth / 2 * (1 + transformX / transformY);
            float scale = (float) constants::RENDER_DISTANCE / transformY;
            if (scale < 0)
                continue;

            spriteInstance->setPosition(screenX, windowHeight / 2 - scale);
            spriteInstance->setOrigin(
                    sprite.sprite.getTexture()->getSize().x / 2,
                    sprite.sprite.getTexture()->getSize().y / 2
                    );

            float scaleX = (float) scale * (sprite.size.x / sprite.sprite.getTexture()->getSize().x);
            float scaleY = (float) scale * (sprite.size.y / sprite.sprite.getTexture()->getSize().y);
            spriteInstance->setScale(scaleX, scaleY);
            zBuffer.emplace_back(spriteInstance, spriteDistance);
        }

        sort(zBuffer.begin(), zBuffer.end(), [](const std::pair<sf::Sprite*, double>& a, const std::pair<sf::Sprite*, double>& b) {
            return a.second > b.second;
        });

        for (auto& sprite: zBuffer) {
            window.draw(*sprite.first);
            delete sprite.first;
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