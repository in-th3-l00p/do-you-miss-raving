//
// Created by intheloop on 10.01.2024.
//

#include "Scene.h"
#include "Map.h"

namespace engine {
    Scene::Scene(sf::RenderWindow &window) : window(window) {}

    TestScene::TestScene(sf::RenderWindow &window) : Scene(window) {
        container.insert(std::make_unique<game::TestMap>(10, 10));
    }

    void TestScene::update(float deltaTime) {
        Scene::update(deltaTime);
    }
} // engine