//
// Created by intheloop on 24.01.2024.
//

#ifndef RAYGAME_SPRITEENTITY_H
#define RAYGAME_SPRITEENTITY_H

#include <set>
#include <map>
#include <deque>
#include <future> // puffin on zooties
#include "Player.h"
#include "Map.h"

namespace engine {
    class SpriteEntity: public Entity, public Sprite {
    public:
        SpriteEntity(
                std::set<std::unique_ptr<Entity>>& container,
                std::map<std::string, Entity*>& labels,
                std::string path,
                math::Vec2<float> position,
                math::Vec2<float> size
                );
        ~SpriteEntity() override = default;
    };

    class StaticSprite: public SpriteEntity {
    public:
        StaticSprite(
                std::set<std::unique_ptr<Entity>>& container,
                std::map<std::string, Entity*>& labels,
                std::string path,
                math::Vec2<float> position,
                math::Vec2<float> size
        );
        ~StaticSprite() override = default;

        void update(float deltaTime) override {};
        void render(sf::RenderWindow& window) override {};
    };

    class Enemy: public SpriteEntity {
    private:
        Player& player;
        Map& map;
        float speed = constants::ENEMY_SPEED;

        math::Vec2<float> target;

        std::deque<math::Vec2<float>> path;
        bool calculatingPath = false;
        std::future<void> pathFuture;

    public:
        Enemy(
                std::set<std::unique_ptr<Entity>>& container,
                std::map<std::string, Entity*>& labels,
                std::string path,
                math::Vec2<float> position,
                math::Vec2<float> size,
                Player& player,
                Map& map
        );
        ~Enemy() override = default;

        void update(float deltaTime) override;
        void render(sf::RenderWindow& window) override;

        void setTarget(math::Vec2<float> target);
    };
} // engine

#endif //RAYGAME_SPRITEENTITY_H
