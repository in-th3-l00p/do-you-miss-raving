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
    class Texture {
    private:
        std::vector<sf::Texture> textureColumns;
        ull width, height;

    public:
        explicit Texture(const std::string& path);
        sf::Texture& operator[](int index);

        [[nodiscard]] ull getWidth() const;
        [[nodiscard]] ull getHeight() const;
    };


    struct Tile {
        std::unique_ptr<Texture> texture;
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
