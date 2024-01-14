//
// Created by intheloop on 10.01.2024.
//

#include "Map.h"
#include "../../utils/Paths.h"

namespace game {
    Texture::Texture(const std::string &path) {
        sf::Image image;
        image.loadFromFile(path);
        width = image.getSize().x;
        height = image.getSize().y;
        for (int i = 0; i < image.getSize().x; i++) {
            sf::Texture texture;
            texture.loadFromImage(
                    image,
                    sf::IntRect(i, 0, 1, (int) height)
                    );
            textureColumns.push_back(texture);
        }
    }

    sf::Texture &Texture::operator[](int index) {
        if (index < 0 && index >= textureColumns.size())
            throw std::out_of_range("Index out of range");
        return textureColumns[index];
    }

    ull Texture::getWidth() const {
        return width;
    }

    ull Texture::getHeight() const {
        return height;
    }


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
            tiles[i][0].texture = std::make_unique<Texture>(engine::paths::WOLFSTEIN_PATH / "wood.png");
            tiles[i][height - 1].texture = std::make_unique<Texture>(engine::paths::WOLFSTEIN_PATH / "wood.png");
            tiles[i][0].empty = false;
            tiles[i][height - 1].empty = false;
        }

        // Set the color of the tiles in the first and last column to red
        for (ull j = 0; j < height; ++j) {
            tiles[0][j].texture = std::make_unique<Texture>(engine::paths::WOLFSTEIN_PATH / "wood.png");
            tiles[width - 1][j].texture = std::make_unique<Texture>(engine::paths::WOLFSTEIN_PATH / "wood.png");
            tiles[0][j].empty = false;
            tiles[width - 1][j].empty = false;
        }

        tiles[5][3].texture = std::make_unique<Texture>(engine::paths::WOLFSTEIN_PATH / "wood.png");
        tiles[5][3].empty = false;
        tiles[2][4].texture = std::make_unique<Texture>(engine::paths::WOLFSTEIN_PATH / "wood.png");
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
//                window.draw(tile);
            }
        }
    }

    void TestMap::update(float deltaTime) {

    }
} // game