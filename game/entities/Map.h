//
// Created by intheloop on 10.01.2024.
//

#ifndef RAYGAME_MAP_H
#define RAYGAME_MAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "../../engine/Entity.h"
#include "../../utils/Constants.h"

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
        Map(ull width, ull height, ull tileSize = engine::constants::DEFAULT_TILESIZE);
        ~Map() override = default;

        [[nodiscard]] const std::vector<std::vector<Tile>> &getTiles() const;

        [[nodiscard]] ull getTileSize() const;
        [[nodiscard]] ull getWidth() const;
        [[nodiscard]] ull getHeight() const;
    };

    class TestMap: public Map {
    public:
        TestMap(ull width, ull height, ull tileSize = engine::constants::DEFAULT_TILESIZE);
        void render(sf::RenderWindow& window) override;
        void update(float deltaTime) override;
    };
} // game

#endif //RAYGAME_MAP_H
