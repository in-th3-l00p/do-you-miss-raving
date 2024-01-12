//
// Created by intheloop on 10.01.2024.
//

#ifndef RAYGAME_SCENE_H
#define RAYGAME_SCENE_H

#include <iostream>
#include <set>
#include <list>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include "Entity.h"

namespace engine {
    // base virtual class
    class Scene {
    protected:
        sf::RenderWindow& window;
        std::unique_ptr<Scene>& sceneRef;

        std::set<std::unique_ptr<Entity>> container;
        std::list<Entity*> renderQueue;

        void addEntity(std::unique_ptr<Entity> entity);

    public:
        explicit Scene(
                sf::RenderWindow &window,
                std::unique_ptr<Scene>& sceneRef
                );

        virtual void update(float deltaTime);
    };

    class TestScene: public Scene {
    public:
        explicit TestScene(
                sf::RenderWindow &window,
                std::unique_ptr<Scene>& sceneRef
                );

        void update(float deltaTime) override;
    };
} // engine

#endif //RAYGAME_SCENE_H
