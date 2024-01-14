//
// Created by intheloop on 10.01.2024.
//

#include "Scene.h"
#include "../game/entities/Map.h"
#include "../game/entities/Player.h"
#include "../game/graphics/Raycaster.h"
#include "../game/graphics/UserInterface.h"

namespace engine {
    Scene::Scene(
            sf::RenderWindow &window,
            std::unique_ptr<Scene>& sceneRef
            ) : window(window), sceneRef(sceneRef) {

    }

    void Scene::addEntity(std::unique_ptr<Entity> entity) {
        int lo = 0, hi = (int)renderQueue.size() - 1;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;

            auto it = renderQueue.begin();
            std::advance(it, mid);
            Entity* currentEntity = *it;

            if (currentEntity->getZIndex() < entity->getZIndex())
                lo = mid + 1;
            else
                hi = mid - 1;
        }

        Entity* entityPtr = entity.get();
        container.insert(std::move(entity));
        renderQueue.insert(std::next(renderQueue.begin(), lo), entityPtr);
    }

    void Scene::update(float deltaTime) {
        for (auto& entity: container)
            entity->update(deltaTime);

        for (auto& entity: renderQueue)
            entity->render(window);
    }

    TestScene::TestScene(
            sf::RenderWindow &window,
            std::unique_ptr<Scene>& sceneRef
            ) : Scene(window, sceneRef) {
        std::unique_ptr<engine::Entity> map = std::make_unique<game::TestMap>(10, 10);
        std::unique_ptr<engine::Entity> player = std::make_unique<game::Player>();
        auto raycaster = std::make_unique<Raycaster>(
                dynamic_cast<game::Player&>(*player),
                dynamic_cast<game::Map&>(*map)
                );
        addEntity(std::move(map));
        addEntity(std::move(player));
        addEntity(std::move(raycaster));
    }

    void TestScene::update(float deltaTime) {
        Scene::update(deltaTime);
    }
} // engine