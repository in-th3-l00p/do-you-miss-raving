//
// Created by intheloop on 10.01.2024.
//

#ifndef RAYGAME_SCENE_H
#define RAYGAME_SCENE_H

#include <iostream>
#include <set>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Entity.h"

namespace engine {
    // base virtual class
    class Scene {
    protected:
        sf::RenderWindow& window;
        std::set<std::unique_ptr<Entity>> container;

    public:
        explicit Scene(sf::RenderWindow &window);

        virtual void update(float deltaTime) {
            for (auto &entity : container) {
                entity->update(deltaTime);
                entity->render(window);
            }
        }
    };

    class TestScene: public Scene {
    public:
        explicit TestScene(sf::RenderWindow &window);

        void update(float deltaTime) override;
    };
} // engine

#endif //RAYGAME_SCENE_H
