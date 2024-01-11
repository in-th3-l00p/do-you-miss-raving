//
// Created by intheloop on 10.01.2024.
//

#ifndef RAYGAME_MAP_H
#define RAYGAME_MAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Entity.h"
#include "config.h"

namespace game {
    struct Tile {
        sf::Color color = sf::Color::White;
        bool empty = true;
    };

    class Map: public engine::Entity {
    protected:
        std::vector<std::vector<Tile>> tiles;
        const ull tileSize;

    public:
        Map(ull width, ull height, ull tileSize = config::DEFAULT_TILESIZE);
        virtual ~Map() = default;

        const std::vector<std::vector<Tile>> &getTiles() const;

        const ull getTileSize() const;
    };

    class TestMap: public Map {
    public:
        TestMap(ull width, ull height, ull tileSize = config::DEFAULT_TILESIZE);
        void render(sf::RenderWindow& window) override;
        void update(float deltaTime) override;
    };
} // game

#endif //RAYGAME_MAP_H
