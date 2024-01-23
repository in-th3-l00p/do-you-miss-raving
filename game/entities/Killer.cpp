#include <iostream>
#include "Killer.h"

namespace game {

    Killer::Killer(std::string path,
                   engine::math::Vec2<float> position,
                   engine::math::Vec2<float> size,
                   Map& map,
                   float speed,
                   Player& player)
            : Sprite(path, position, size), map(map), astar(astar), speed(10), player(player) {
        texture = new sf::Texture();
        bool loadSuccessful = texture->loadFromFile(path);
        if (!loadSuccessful) {
            std::cerr << "Failed to load killer texture from path: " << path << std::endl;
        }
        sprite.setTexture(*texture);
    }

    void Killer::update(float deltaTime) {
        engine::math::Vec2<float> newPathPoint = astar.findPath(position, map);

        engine::math::Vec2<float> direction = newPathPoint - position;
        direction = engine::math::normalize(direction);
        position += direction * speed * deltaTime;

        if (engine::math::distance(position, player.getPosition()) < engine::constants::detectionRange) {
            std::cout << "Player detected! Attack or trigger an event.\n";
        }
    }

    // Implement other member functions if needed...

} // namespace game