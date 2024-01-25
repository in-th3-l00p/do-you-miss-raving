//
// Created by intheloop on 10.01.2024.
//

#include <iostream>
#include "Map.h"
#include "../../utils/Paths.h"

namespace engine {
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

    Map::Map(
            const std::set<std::unique_ptr<Entity>>& container,
            const std::map<std::string, Entity*>& labels,
            ull width, ull height
            ): Entity(container, labels) {
        setZIndex(100);
        tiles.resize(width);
        for (auto &row : tiles)
            row.resize(height);
    }

    const std::vector<std::vector<Tile>> &Map::getTiles() const {
        return tiles;
    }

    const std::vector<Sprite*> &Map::getSprites() const {
        return sprites;
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

    void Map::addSprite(Sprite* sprite) {
        sprites.push_back(sprite);
    }

    TestMap::TestMap(
            const std::set<std::unique_ptr<Entity>>& container,
            const std::map<std::string, Entity*>& labels,
            ull width, ull height
            ) : Map(container, labels, width, height) {

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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
            for (auto &sprite: sprites) {
                sf::RectangleShape background;
                background.setPosition(sprite->position.x, sprite->position.y);
                background.setSize(sf::Vector2f(sprite->size.x, sprite->size.y));
                background.setFillColor(sf::Color::Magenta);
                window.draw(background);

                sprite->sprite.setPosition(sprite->position.x, sprite->position.y);
                sprite->sprite.setScale(sprite->size.x / sprite->texture->getSize().x,
                                        sprite->size.y / sprite->texture->getSize().y);
                window.draw(sprite->sprite);

                sf::RectangleShape point;
                point.setPosition(sprite->position.x, sprite->position.y);
                point.setSize(sf::Vector2f(1, 1));
                point.setFillColor(sf::Color::Green);
                window.draw(point);
            }
        }
    }

    void TestMap::update(float deltaTime) {

    }
} // engine