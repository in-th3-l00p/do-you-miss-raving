//
// Created by intheloop on 12.01.2024.
//

#include <SFML/Graphics.hpp>
#include "Raycaster.h"
#include "../../utils/Math.h"
#include <iostream>
#define texWidth 64

namespace engine {
    engine::Raycaster::Raycaster(game::Player &player, game::Map &map, game::SSprite &enemy):
            player(player), map(map), enemy(enemy) {
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
                if (distance > constants::RENDER_DISTANCE)
                    continue;

                texture = &map.getTiles()[horizontal.tile.y][horizontal.tile.x].texture;
                double multiplier = horizontal.hit.y / (double) map.getTileSize() - horizontal.tile.y;
                int hit = std::floor(engine::math::linearInterpolation<double>(multiplier, 0, 1, 0, (double) texture->getSize().y));
                line->setTexture(*texture);
                line->setTextureRect(sf::IntRect(hit, 0, 1, texture->getSize().y));
            } else {
                distance = vertical.distance;
                if (distance > constants::RENDER_DISTANCE)
                    continue;

                texture = &map.getTiles()[vertical.tile.y][vertical.tile.x].texture;
                double multiplier = vertical.hit.x / (double) map.getTileSize() - vertical.tile.x;
                int hit = std::floor(engine::math::linearInterpolation<double>(multiplier, 0, 1, 0, (double) texture->getSize().x));
                line->setTexture(*texture);
                line->setTextureRect(sf::IntRect(hit, 0, 1, texture->getSize().y));

                color = std::floor((float) color * constants::HORIZONTAL_DARKER_MUTLIPLIER);
            }

            // lighting
            color = (int)((float) color * (float) (1 - distance / constants::RENDER_DISTANCE ));
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

            spriteInstance->setPosition(screenX, (float) windowHeight / 2 - scale / 2);
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

//    void engine::Raycaster::renderSprites(sf::RenderWindow& window)
//    {
//        spriteDistance = ((player.getPosition().x - enemy.getPosition().x) * (player.getPosition().x - enemy.getPosition().x) + (player.getPosition().y - enemy.getPosition().y) * (player.getPosition().y - enemy.getPosition().y)); //sqrt not taken, unneeded;
//            double spriteX = enemy.getPosition().x - player.getPosition().x;
//            double spriteY = enemy.getPosition().y - player.getPosition().y;;
//
//            //transform sprite with the inverse camera matrix
//            // [ planeX   dirX ] -1                                       [ dirY      -dirX ]
//            // [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
//            // [ planeY   dirY ]                                          [ -planeY  planeX ]
//
//            double invDet = 1.0 / (player.getCameraPlane().x * player.getDirection().y - player.getDirection().x * player.getCameraPlane().y); //required for correct matrix multiplication
//
//            double transformX = invDet * (player.getDirection().y * spriteX - player.getDirection().x * spriteY);
//            double transformY = invDet * (-player.getCameraPlane().y * spriteX + player.getCameraPlane().x * spriteY); //this is actually the depth inside the screen, that what Z is in 3D
//
//            int w= enemy.getWidth(window);
//            int h= enemy.getWidth(window);
//            int spriteScreenX = int((w / 2) * (1 + transformX / transformY));
//
//            //calculate height of the sprite on screen
//            int spriteHeight = abs(int(enemy.getHeight(window) / (transformY))); //using 'transformY' instead of the real distance prevents fisheye
//            //calculate lowest and highest pixel to fill in current stripe
//            int drawStartY = -spriteHeight / 2 + h / 2;
//            if(drawStartY < 0) drawStartY = 0;
//            int drawEndY = spriteHeight / 2 + h / 2;
//            if(drawEndY >= h) drawEndY = h - 1;
//
//            //calculate width of the sprite
//            int spriteWidth = abs( int (h / (transformY)));
//            int drawStartX = -spriteWidth / 2 + spriteScreenX;
//            if(drawStartX < 0) drawStartX = 0;
//            int drawEndX = spriteWidth / 2 + spriteScreenX;
//            if(drawEndX >= w) drawEndX = w - 1;
//
//            //loop through every vertical stripe of the sprite on screen
//            for(int stripe = drawStartX; stripe < drawEndX; stripe++)
//            {
//
//                int circleX = stripe - drawStartX;
//
//                int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
//
//                //the conditions in the if are:
//                if (circleX >= 0 && circleX < spriteWidth && transformY > 0 && stripe > 0 && stripe < w && transformY < ZBuffer[stripe]) {
//                    for (int y = drawStartY; y < drawEndY; y++) //for every pixel of the current stripe
//                    {
//                        int d = (y) * 256 - h * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
//                        int texY = ((d * texHeight) / spriteHeight) / 256;
//                        int circleY = y - drawStartY;
//                        if (circleX * circleX + circleY * circleY < (spriteWidth / 2) * (spriteWidth / 2))
//                        {
//                            // Draw a colored pixel for the circle
//                            Uint32 color = // Specify your color for the circle here;
//                                    buffer[y][stripe] = color;
//                        }
//                }
//            }
//        }

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