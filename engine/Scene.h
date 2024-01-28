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

        std::list<Entity*> renderQueue;
        std::set<std::unique_ptr<Entity>> container;
        std::map<std::string, Entity*> labels;

        void addEntity(std::unique_ptr<Entity> entity);
        void addEntity(std::unique_ptr<Entity> entity, const std::string& label);

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

    class EditorMapScene: public Scene {
    public:
        explicit EditorMapScene(
                sf::RenderWindow &window,
                std::unique_ptr<Scene>& sceneRef,
                std::string id
                );

        void update(float deltaTime) override;
    };
} // engine

#endif //RAYGAME_SCENE_H
