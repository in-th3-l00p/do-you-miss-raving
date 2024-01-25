//
// Created by intheloop on 10.01.2024.
//

#include "Scene.h"
#include "../game/entities/Map.h"
#include "../game/entities/Player.h"
#include "../game/graphics/Raycaster.h"
#include "../game/graphics/UserInterface.h"
#include "../game/entities/SpriteEntity.h"

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

    void Scene::addEntity(std::unique_ptr<Entity> entity, const std::string &label) {
        Entity* ref = entity.get();
        addEntity(std::move(entity));
        labels[label] = ref;
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
        std::unique_ptr<engine::Map> map = std::make_unique<engine::TestMap>(container, labels, 10, 10);
        addEntity(std::move(map), "map");

        std::unique_ptr<engine::Entity> player = std::make_unique<engine::Player>(container, labels);
        addEntity(std::move(player), "player");

        auto raycaster = std::make_unique<Raycaster>(
                container, labels
        );
        addEntity(std::move(raycaster), "raycaster");

        auto enemy = std::make_unique<engine::Enemy>(
                container, labels,
                (paths::IMAGES_PATH / "iosub.png").string(),
                math::Vec2<float>(200, 200),
                math::Vec2<float>(constants::DEFAULT_TILESIZE, constants::DEFAULT_TILESIZE),
                dynamic_cast<engine::Player&>(*labels.at("player")),
                dynamic_cast<engine::Map&>(*labels.at("map"))
                );
        ((Map*)labels["map"])->addSprite(enemy.get());
        addEntity(std::move(enemy), "enemy");

        auto barChair = std::make_unique<engine::StaticSprite>(
                container, labels,
                (paths::SPRITES_PATH / "bar chair.png").string(),
                math::Vec2<float>(360, 260),
                math::Vec2<float>((float) constants::DEFAULT_TILESIZE / 2, (float) constants::DEFAULT_TILESIZE)
                );
        ((Map*)labels["map"])->addSprite(barChair.get());
        addEntity(std::move(barChair), "barChair");

        auto disco = std::make_unique<engine::StaticSprite>(
                container, labels,
                (paths::SPRITES_PATH / "disco1.png").string(),
                math::Vec2<float>(150, 150),
                math::Vec2<float>((float) constants::DEFAULT_TILESIZE, (float) constants::DEFAULT_TILESIZE)
                );
//        ((Map*)labels["map"])->addSprite(disco.get());
//        addEntity(std::move(disco), "disco");
    }

    void TestScene::update(float deltaTime) {
        Scene::update(deltaTime);
    }
} // engine