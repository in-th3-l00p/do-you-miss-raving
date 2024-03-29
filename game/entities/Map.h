//
// Created by intheloop on 10.01.2024.
//

#ifndef RAYGAME_MAP_H
#define RAYGAME_MAP_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../../engine/Entity.h"
#include "../../utils/Constants.h"
#include "../../utils/Math.h"
#include "../../utils/Paths.h"

namespace engine {
    // to do: fix memory leak
    struct Sprite {
        sf::Texture *texture;
        std::unique_ptr<sf::Sprite> sprite;
        engine::math::Vec2<float> position; // ruxy
        engine::math::Vec2<float> size;

        Sprite(std::string path, engine::math::Vec2<float> position, engine::math::Vec2<float> size);
        ~Sprite() = default;
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
        const ull tileSize = engine::constants::DEFAULT_TILESIZE;

        std::vector<Sprite*> sprites;

        math::Vec2<float> playerStartPosition;
        math::Vec2<float> playerStartDirection;
        math::Vec2<float> enemyStartPosition;

    public:
        Map(
                const std::set<std::unique_ptr<Entity>>& container,
                const std::map<std::string, Entity*>& labels,
                ull width,
                ull height
        );
        ~Map() override = default;

        [[nodiscard]] const std::vector<std::vector<Tile>> &getTiles() const;

        [[nodiscard]] const std::vector<Sprite*>& getSprites() const;
        void addSprite(Sprite* sprite);

        [[nodiscard]] ull getTileSize() const;
        [[nodiscard]] ull getWidth() const;
        [[nodiscard]] ull getHeight() const;

        Tile & getTile(int x, int y);

        const math::Vec2<float> &getPlayerStartPosition() const;

        void setPlayerStartPosition(const math::Vec2<float> &playerStartPosition);

        const math::Vec2<float> &getPlayerStartDirection() const;

        void setPlayerStartDirection(const math::Vec2<float> &playerStartDirection);

        const math::Vec2<float> &getEnemyStartPosition() const;

        void setEnemyStartPosition(const math::Vec2<float> &enemyStartPosition);

        void render(sf::RenderWindow& window) override {};
        void update(float deltaTime) override {};
    };

    class TestMap: public Map {
    public:
        TestMap(
                const std::set<std::unique_ptr<Entity>>& container,
                const std::map<std::string, Entity*>& labels,
                ull width, ull height
            );
        void render(sf::RenderWindow& window) override;
        void update(float deltaTime) override;
    };
} // engine

#endif //RAYGAME_MAP_H
