//
// Created by intheloop on 10.01.2024.
//

#include "Map.h"
#include "../../utils/Paths.h"

namespace game {
    Sprite::Sprite(
            std::string path,
            engine::math::Vec2<float> position,
            engine::math::Vec2<float> size
    ): position(position), size(size) {
        texture = new sf::Texture();
        texture->loadFromFile(path);
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
        sprites.emplace_back(
                (engine::paths::IMAGES_PATH / "iosub.png").string(),
                engine::math::Vec2<float>{ 300, 300 },
                engine::math::Vec2<float>{ (float) tileSize, (float) tileSize }
                );

        sprites.emplace_back(
                (engine::paths::IMAGES_PATH / "gimnastica.jpeg").string(),
                engine::math::Vec2<float>{ 460, 300 },
                engine::math::Vec2<float>{ (float) tileSize * 3, (float) tileSize * 3 }
        );

        sprites.emplace_back(
                (engine::paths::IMAGES_PATH / "iosub_aripi_devant.png").string(),
                engine::math::Vec2<float>{ 460, 460 },
                engine::math::Vec2<float>{ (float) tileSize, (float) tileSize }
        );

        sprites.emplace_back(
                (engine::paths::IMAGES_PATH / "luci.jpeg").string(),
                engine::math::Vec2<float>{ 300, 360 },
                engine::math::Vec2<float>{ (float) tileSize, (float) tileSize }
        );

        sprites.emplace_back(
                (engine::paths::IMAGES_PATH / "capu ba.png").string(),
                engine::math::Vec2<float>{ 100, 100 },
                engine::math::Vec2<float>{ (float) tileSize, (float) tileSize }
        );

        sprites.emplace_back(
                (engine::paths::IMAGES_PATH / "floricel.png").string(),
                engine::math::Vec2<float>{ 200, 100 },
                engine::math::Vec2<float>{ (float) tileSize, (float) tileSize }
        );

        // Set the color of the tiles in the first and last row to red
        for (ull i = 0; i < width; ++i) {
            tiles[i][0].texture.loadFromFile((engine::paths::IMAGES_PATH / "iosubugrasu.png").string());
            tiles[i][height - 1].texture.loadFromFile((engine::paths::IMAGES_PATH / "iosubugrasu.png").string());
            tiles[i][0].empty = false;
            tiles[i][height - 1].empty = false;
        }

        // Set the color of the tiles in the first and last column to red
        for (ull j = 0; j < height; ++j) {
            tiles[0][j].texture.loadFromFile((engine::paths::IMAGES_PATH / "iosubugrasu.png").string());
            tiles[width - 1][j].texture.loadFromFile((engine::paths::IMAGES_PATH / "iosubugrasu.png").string());
            tiles[0][j].empty = false;
            tiles[width - 1][j].empty = false;
        }

        tiles[5][3].texture.loadFromFile((engine::paths::IMAGES_PATH / "jumpscare.jpg").string());
        tiles[5][3].empty = false;
        tiles[2][4].texture.loadFromFile((engine::paths::IMAGES_PATH / "jumpscare.jpg").string());
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
    }

    void TestMap::update(float deltaTime) {

    }
} // game