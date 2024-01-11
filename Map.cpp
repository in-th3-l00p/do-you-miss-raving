//
// Created by intheloop on 10.01.2024.
//

#include "Map.h"

namespace game {
    Map::Map(ull width, ull height, ull tileSize): tileSize(tileSize) {
        tiles.resize(width);
        for (auto &row : tiles)
            row.resize(height);
    }

    const std::vector<std::vector<Tile>> &Map::getTiles() const {
        return tiles;
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

    TestMap::TestMap(ull width, ull height, ull tileSize) : Map(width, height, tileSize) {
        // Set the color of the tiles in the first and last row to red
        for (ull i = 0; i < width; ++i) {
            tiles[i][0].color = sf::Color::Red;
            tiles[i][height - 1].color = sf::Color::Red;
            tiles[i][0].empty = false;
            tiles[i][height - 1].empty = false;
        }

        // Set the color of the tiles in the first and last column to red
        for (ull j = 0; j < height; ++j) {
            tiles[0][j].color = sf::Color::Red;
            tiles[width - 1][j].color = sf::Color::Red;
            tiles[0][j].empty = false;
            tiles[width - 1][j].empty = false;
        }
    }

    void TestMap::render(sf::RenderWindow &window) {
        for (ull i = 0; i < tiles.size(); ++i) {
            for (ull j = 0; j < tiles[i].size(); ++j) {
                sf::RectangleShape tile;
                tile.setSize(sf::Vector2f(tileSize, tileSize));
                tile.setPosition(i * tileSize, j * tileSize);
                tile.setFillColor(tiles[i][j].color);
                window.draw(tile);
            }
        }
    }

    void TestMap::update(float deltaTime) {

    }
} // game