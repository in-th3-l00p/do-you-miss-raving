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
        // Add killer sprites
        for (const auto& killer : killers) {
            allSprites->push_back(killer);
        }
        // Add decoration sprites
        for (const auto& decoration : decorations) {
            allSprites->push_back(decoration);
        }

        for (const auto& sprite : sprites) {
            allSprites->push_back(sprite);
        }

        // Add other sprites as needed

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

    Killer::Killer(std::string path, engine::math::Vec2<float> position, engine::math::Vec2<float> size)
            : Sprite(path, position, size)
    {
        texture = new sf::Texture();
        bool loadSuccessful = texture->loadFromFile(path);
        if (!loadSuccessful) {
            std::cerr << "Failed to load killer texture from path: " << path << std::endl;
        }
        sprite.setTexture(*texture);
    }

    void Killer::update(float deltaTime) {

//        engine::math::Vec2<float> playerPosition = {
//                player.getPosition().x,
//                player.getPosition().y
//        };
//        int tileSize = map.getTileSize();
//
//        engine::math::Vec2<int> playerTile = {
//                static_cast<int>(playerPosition.x /  tileSize),
//                static_cast<int>(playerPosition.y / tileSize)
//        };
//
//        engine::math::Vec2<int> killerTile = {
//                static_cast<int>(position.x / tileSize),
//                static_cast<int>(position.y / tileSize)
//        };
//
//        if (playerTile == killerTile) {
//            std::cout<<"hatz\n";
//        } else {
//            auto heuristic = [](int x, int y, const engine::math::Vec2<int>& target) {
//                return std::sqrt(static_cast<float>((x - target.x) * (x - target.x) + (y - target.y) * (y - target.y)));
//            };
//
//            // Calculate cost from current to neighbor (you need to replace this with your actual cost calculation)
//            auto calculateCost = [](int /*currentX*/, int /*currentY*/, int /*neighborX*/, int /*neighborY*/) {
//                return 1.0f;  // Replace this with your actual cost calculation
//            };
//            // Perform A* pathfinding
//            openSet.push({killerTile.x, killerTile.y, 0.0f, 0.0f});
//
//            while (!openSet.empty()) {
//                Node current = openSet.top();
//                openSet.pop();
//
//                // Check if the current node is the target
//                if (current.x == playerTile.x && current.y == playerTile.y) {
//                    // Found the target, move towards it
//                    position.x = current.x * tileSize;
//                    position.y = current.y * tileSize;
//                    break;
//                }
//
//                // Process neighbors (adjacent tiles)
//                for (int i = -1; i <= 1; ++i) {
//                    for (int j = -1; j <= 1; ++j) {
//                        if (i == 0 && j == 0) continue;
//
//                        int neighborX = current.x + i;
//                        int neighborY = current.y + j;
//
//                        if (isValidTile(neighborX, neighborY)) {
//                            float newG = current.g + calculateCost(current.x, current.y, neighborX, neighborY);
//                            addToOpenSet(neighborX, neighborY, newG, playerTile, heuristic);
//                        }
//                    }
//                }
//
//                closedSet.insert(current.x * map.getWidth() + current.y);
//            }
//
//            // Clear open and closed sets for the next update
//            openSet = std::priority_queue<Node, std::vector<Node>, std::greater<Node>>();
//            closedSet.clear();
//        }
    }

//    void Killer::addToOpenSet(int x, int y, float g, const engine::math::Vec2<int>& target, HeuristicFunction heuristic) {
//        if (closedSet.find(x * map.getWidth() + y) == closedSet.end()) {
//            float h = heuristic(x, y, target);  // Calculate heuristic
//            float f = g + h;
//            openSet.push({x, y, f, g});
//        }
//    }
//
//    bool Killer::isValidTile(int x, int y) {
//        return x >= 0 && x < map.getWidth() && y >= 0 && y < map.getHeight() && map.getTiles()[y][x].empty;
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
        killers.emplace_back(
                (engine::paths::SPRITES_PATH / "bar chair.png").string(),
                engine::math::Vec2<float>{120, 300},
                engine::math::Vec2<float>{(float) tileSize / 2.f, (float) tileSize / 2}
        );
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

        for (auto &killer : killers) {
            killer.sprite.setPosition(killer.position.x, killer.position.y);
            killer.sprite.setScale(killer.size.x / killer.texture->getSize().x,
                                   killer.size.y / killer.texture->getSize().y);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
                window.draw(killer.sprite);
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

            for (auto& killer : killers) {
                killer.update(deltaTime);
            }

            for (auto& decoration : decorations) {
                decoration.update(deltaTime);
            }
    }
} // game