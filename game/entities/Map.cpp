//
// Created by intheloop on 10.01.2024.
//

#include <iostream>
#include "Map.h"

namespace game {

    Sprite::Sprite(
            std::string path,
            engine::math::Vec2<float> position,
            engine::math::Vec2<float> size
    ): position(position), size(size) {
        texture = new sf::Texture();
        bool loadSuccessful = texture->loadFromFile(path);
        if (!loadSuccessful) {
            std::cerr << "Failed to load texture from path: " << path << std::endl;
        }
        sprite.setTexture(*texture);
    }

    Map::Map(ull width, ull height, ull tileSize): tileSize(tileSize) {
        setZIndex(100);
        tiles.resize(width);
        for (auto &row : tiles)
            row.resize(height);
    }

    const std::vector<std::vector<Tile>> &Map::getTiles() const {
        return tiles;
    }

    const std::vector<Sprite> &Map::getSprites() const {
        return sprites;
    }

    std::shared_ptr<std::vector<Sprite>> Map::getAllSprites() const {
        auto allSprites = std::make_shared<std::vector<Sprite>>();
//        for (const Killer& killer : killers) {
//            allSprites->push_back(static_cast<const Sprite&>((const Sprite &) killer));
//        }

        for (const auto& decoration : decorations) {
            allSprites->push_back(decoration);
        }

        for (const auto& sprite : sprites) {
            allSprites->push_back(sprite);
        }
        return allSprites;
    }


    ull Map::getTileSize() const {
        return tileSize;
    }

    ull Map::getWidth() const {
        return tiles[0].size();
    }

    ull Map::getHeight() const {
        return tiles.size();
    }

    Tile & Map::getTile(int i, int i1) {
        return tiles[i][i1];
    }

    void Sprite::update(float deltaTime) {
    }

//    Killer::Killer(std::string path, engine::math::Vec2<float> position, engine::math::Vec2<float> size, Map& map)
//            : Sprite(path, position, size), map(map)
//    {
//        texture = new sf::Texture();
//        bool loadSuccessful = texture->loadFromFile(path);
//        if (!loadSuccessful) {
//            std::cerr << "Failed to load killer texture from path: " << path << std::endl;
//        }
//        sprite.setTexture(*texture);
//    }
//
//    void Killer::update(float deltaTime) {
//        engine::math::Vec2<float> newPathPoint = astar.findPath(position, map);
//
//        engine::math::Vec2<float> direction = newPathPoint - position;
//        direction = engine::math::normalize(direction);
//        position += direction * speed * deltaTime;
//
//        if (engine::math::distance(position, playerPosition) < detectionRange) {
//            // Perform actions when the player is detected
//            // You might want to attack the player, trigger an event, etc.
//            // This is just a placeholder; replace it with your actual logic
//            std::cout << "Player detected! Attack or trigger an event.\n";
//        }
//    }

    Decoration::Decoration(std::string path, engine::math::Vec2<float> position, engine::math::Vec2<float> size)
            : Sprite(path, position, size)
    {
        texture = new sf::Texture();
        bool loadSuccessful = texture->loadFromFile(path);
        if (!loadSuccessful) {
            std::cerr << "Failed to load killer texture from path: " << path << std::endl;
        }
        sprite.setTexture(*texture);
    }

    void Decoration::update(float deltaTime) {
        // Add specific logic for decoration movement here
        // For example, rotate the decoration, change opacity, etc.
        // This is just a placeholder, replace it with your actual logic
        sprite.rotate(10 * deltaTime); // Adjust the rotation speed
    }

    TestMap::TestMap(ull width, ull height, ull tileSize) : Map(width, height, tileSize) {
        decorations.emplace_back(
                (engine::paths::SPRITES_PATH / "bar chair.png").string(),
                engine::math::Vec2<float>{100, 100},
                engine::math::Vec2<float>{(float) tileSize / 2.f, (float) tileSize / 2}
        );

        sprites.emplace_back(
                (engine::paths::SPRITES_PATH / "bar chair.png").string(),
                engine::math::Vec2<float>{ 360, 260 },
                engine::math::Vec2<float>{ (float) tileSize / 2.f, (float) tileSize / 2 }
        );

        // Set the color of the tiles in the first and last row to red
        for (ull i = 0; i < width; ++i) {
                tiles.at(i).at(0).changeTexture((engine::paths::RETRO_PATH / "CONCRETE_1A.PNG").string());
                tiles.at(i).at(height - 1).changeTexture((engine::paths::RETRO_PATH / "CONCRETE_1A.PNG").string());
                tiles.at(i).at(0).empty = false;
                tiles.at(i).at(height - 1).empty = false;
        }

        // Set the color of the tiles in the first and last column to red
        for (ull j = 0; j < height; ++j) {
            tiles[0][j].changeTexture((engine::paths::RETRO_PATH / "CONCRETE_1A.PNG").string());
            tiles[width - 1][j].changeTexture((engine::paths::RETRO_PATH / "CONCRETE_1A.PNG").string());
            tiles[0][j].empty = false;
            tiles[width - 1][j].empty = false;
        }

        tiles[5][3].changeTexture((engine::paths::WOLFSTEIN_PATH / "wood.png").string());
        tiles[5][3].empty = false;
        tiles[2][4].changeTexture((engine::paths::WOLFSTEIN_PATH / "wood.png").string());
        tiles[2][4].empty = false;
    }

    void TestMap::render(sf::RenderWindow &window) {
        for (ull i = 0; i < tiles.size(); ++i) {
            for (ull j = 0; j < tiles[i].size(); ++j) {
                sf::RectangleShape tile;
                tile.setSize(sf::Vector2f(tileSize, tileSize));
                tile.setPosition(j * tileSize, i * tileSize);
                if (tiles[i][j].empty)
                    tile.setFillColor(sf::Color::Black);
                else
                    tile.setFillColor(sf::Color::Red);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
                    window.draw(tile);
            }
        }

        for (auto &sprite : sprites) {
            sprite.sprite.setPosition(sprite.position.x, sprite.position.y);
            sprite.sprite.setScale(sprite.size.x / sprite.texture->getSize().x,
                                   sprite.size.y / sprite.texture->getSize().y);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
                window.draw(sprite.sprite);
        }

        // Render decoration sprites
        for (auto &decoration : decorations) {
            decoration.sprite.setPosition(decoration.position.x, decoration.position.y);
            decoration.sprite.setScale(decoration.size.x / decoration.texture->getSize().x,
                                       decoration.size.y / decoration.texture->getSize().y);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
                window.draw(decoration.sprite);
        }
    }
        void TestMap::update(float deltaTime) {
            for (auto& sprite : sprites) {
                sprite.update(deltaTime);
            }

            for (auto& decoration : decorations) {
                decoration.update(deltaTime);
            }
    }
} // game