//
// Created by intheloop on 24.01.2024.
//

#include "SpriteEntity.h"

#include <utility>
#include <iostream>
#include <queue>

namespace engine {
    SpriteEntity::SpriteEntity(
            std::set<std::unique_ptr<Entity>> &container,
            std::map<std::string, Entity *> &labels,
            std::string path,
            math::Vec2<float> position,
            math::Vec2<float> size
    ): Entity(container, labels), Sprite(std::move(path), position, size) {
    }

    Enemy::Enemy(
            std::set<std::unique_ptr<Entity>> &container,
            std::map<std::string, Entity *> &labels,
            std::string path,
            math::Vec2<float> position,
            math::Vec2<float> size,
            Player &player,
            Map& map
            ):
        SpriteEntity(container, labels, std::move(path), position, size),
        player(player), map(map) {
    }

    void Enemy::update(float deltaTime) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !calculatingPath)
            pathFuture = std::async(std::launch::async, &Enemy::setTarget, this, player.getPosition());

        if (calculatingPath)
            return;
        float speedLeft = speed * deltaTime;
        std::cout << speedLeft << '\n';
        while (speedLeft > 0 && !path.empty()) {
            math::Vec2<float> next = path.front();
            if (next.getDistance(position) <= speedLeft) {
                position = next;
                path.pop_front();
                speedLeft -= next.getDistance(position);
            } else {
                math::Vec2<float> direction = next - position;
                direction = direction.normalize();
                position += direction * speedLeft;
                speedLeft = 0;
            }
        }
    }

    void Enemy::render(sf::RenderWindow &window) {
    }

    void Enemy::setTarget(math::Vec2<float> target) {
        calculatingPath = true;
        this->target = target;
        path.clear();

        std::queue<math::Vec2<float>> queue;
        std::set<math::Vec2<float>> visited;
        std::map<math::Vec2<float>, math::Vec2<float>> parent;

        queue.push(position);
        visited.insert(position);
        parent[position] = position;

        while (!queue.empty()) {
            math::Vec2<float> current = queue.front();
            queue.pop();

            if (current.getDistance(target) < player.getRadius()) {
                while (current != position) {
                    path.push_front(current);
                    current = parent[current];
                }

                break;
            }

            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    math::Vec2<float> next = current + math::Vec2<float>((float) i, (float) j);
                    math::Vec2<float> points[] = {
                            next,
                            next + math::Vec2<float>(0, size.y),
                            next + math::Vec2<float>(size.x, 0),
                            next + size
                    };

                    if (
                            visited.find(next) == visited.end() &&
                            map.getTile(
                                    (int) (points[0].x / (float) map.getTileSize()),
                                    (int) (points[0].y / (float) map.getTileSize())
                            ).empty &&
                            map.getTile(
                                    (int) (points[1].x / (float) map.getTileSize()),
                                    (int) (points[1].y / (float) map.getTileSize())
                            ).empty &&
                            map.getTile(
                                    (int) (points[2].x / (float) map.getTileSize()),
                                    (int) (points[2].y / (float) map.getTileSize())
                            ).empty &&
                            map.getTile(
                                    (int) (points[3].x / (float) map.getTileSize()),
                                    (int) (points[3].y / (float) map.getTileSize())
                            ).empty
                    ) {
                        queue.push(next);
                        visited.insert(next);
                        parent[next] = current;
                    }
                }
            }
        }

        calculatingPath = false;
    }

    StaticSprite::StaticSprite(
            std::set<std::unique_ptr<Entity>> &container,
            std::map<std::string, Entity *> &labels,
            std::string path,
            math::Vec2<float> position,
            math::Vec2<float> size
            ) : SpriteEntity(container, labels, path, position, size) {
    }
} // engine