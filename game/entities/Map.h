//
// Created by intheloop on 10.01.2024.
//

#ifndef RAYGAME_MAP_H
#define RAYGAME_MAP_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <queue>
#include <unordered_set>
#include <functional>
#include "../../engine/Entity.h"
#include "../../utils/Constants.h"
#include "../../utils/Math.h"
#include "../../utils/Paths.h"
#include "Player.h"

namespace game{
    class Map;
    // to do: fix memory leak
    struct Sprite {
        Sprite(std::string path, engine::math::Vec2<float> position, engine::math::Vec2<float> size);
        sf::Texture *texture;
        sf::Sprite sprite;
        engine::math::Vec2<float> position; // ruxy
        engine::math::Vec2<float> size;
    public:
        virtual ~Sprite() = default;
        void update(float deltaTime);
    };

    struct Node {
        int x, y;
        float f, g;

        bool operator>(const Node& other) const {
            return f > other.f;
        }
    };
    using HeuristicFunction = std::function<float(int, int, const engine::math::Vec2<int>&)>;

    struct Killer : public game::Sprite {
    private:
        std::priority_queue<game::Node, std::vector<game::Node>, std::greater<game::Node>> openSet;
        std::unordered_set<int> closedSet;
        const game::Map& map;
        const game::Player& player;

        void addToOpenSet(int x, int y, float g, const engine::math::Vec2<int>& target, game::HeuristicFunction heuristic);
        bool isValidTile(int x, int y);

    public:
        Killer(std::string path, engine::math::Vec2<float> position, engine::math::Vec2<float> size, game::Map& map, game::Player& player);
        void update(float deltaTime);
    };

    struct Decoration : public Sprite {
    public:
        Decoration(std::string path, engine::math::Vec2<float> position, engine::math::Vec2<float> size);
        void update(float deltaTime);
    };

    struct Tile {
        sf::Texture texture;
        sf::Image image;
        bool empty = true;

        Tile() {
            image.loadFromFile((engine::paths::RETRO_PATH / "FLOOR_1A.PNG").string());
            texture.loadFromImage(image);
        }

        void changeTexture(const std::string& path) {
            image.loadFromFile(path);
            texture.loadFromImage(image);
        }
    };

    class Map: public engine::Entity {
    protected:
        std::vector<std::vector<Tile>> tiles;
        const ull tileSize;

        std::vector<Sprite> sprites;
        std::vector<Killer> killers;
        std::vector<Decoration> decorations;
        std::vector<std::shared_ptr<Sprite>> allSprites;

    public:
        Map(ull width, ull height, ull tileSize = engine::constants::DEFAULT_TILESIZE);
        ~Map() override = default;

        [[nodiscard]] const std::vector<std::vector<Tile>> &getTiles() const;

        const std::vector<Sprite> &getSprites() const;

        std::shared_ptr<std::vector<Sprite>> getAllSprites() const;

        [[nodiscard]] ull getTileSize() const;
        [[nodiscard]] ull getWidth() const;
        [[nodiscard]] ull getHeight() const;

        Tile & getTile(int i, int i1);
    };

    class TestMap: public Map {
    public:
        TestMap(ull width, ull height, Player& player,ull tileSize = engine::constants::DEFAULT_TILESIZE);
        void render(sf::RenderWindow& window) override;
        void update(float deltaTime) override;
    };
} // game

#endif //RAYGAME_MAP_H
