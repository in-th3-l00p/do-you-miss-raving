//
// Created by intheloop on 10.01.2024.
//

#include "Scene.h"
#include "../game/entities/Map.h"
#include "../game/entities/Player.h"
#include "../game/graphics/Raycaster.h"
#include "../game/graphics/UserInterface.h"
#include "../game/entities/SpriteEntity.h"
#include "../../utils/json.hpp"
#include <cpr/cpr.h>

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
                container, labels,
                dynamic_cast<engine::Player&>(*labels.at("player")),
                dynamic_cast<engine::Map&>(*labels.at("map"))
        );
        addEntity(std::move(raycaster), "raycaster");

        auto enemy = std::make_unique<engine::Enemy>(
                container, labels,
                (paths::IMAGES_PATH / "iosub gimnastica.png").string(),
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

    EditorMapScene::EditorMapScene(
            sf::RenderWindow &window,
            std::unique_ptr<Scene> &sceneRef,
            std::string id
    ): Scene(window, sceneRef) {
        cpr::Response response = cpr::Get(cpr::Url{"https://doyoumissraving.intheloop.bio/api/maps/" + id});
        if (response.status_code != 200) {
            std::cerr << "Error: " << response.status_code << std::endl;
            return;
        }

        nlohmann::json data = nlohmann::json::parse(response.text);
        std::unique_ptr<engine::Map> map = std::make_unique<engine::Map>(
                container, labels,
                (ull) data["width"], (ull) data["height"]
        );
        Map& mapRef = *map;
        addEntity(std::move(map), "map");

        std::unique_ptr<Player> player = std::make_unique<Player>(
                container, labels,
                math::Vec2<float>(data["playerStartX"], data["playerStartY"]),
                math::Vec2<float>(data["playerStartDirectionX"], data["playerStartDirectionY"]),
                constants::DEFAULT_PLAYER_RADIUS,
                constants::DEFAULT_PLAYER_SPEED,
                constants::DEFAULT_PLAYER_ROTATE_SPEED
        );
        Player& playerRef = *player;
        addEntity(std::move(player), "player");

        for (auto& texture: data["textures"]) {
            cpr::Response imageResponse = cpr::Get(cpr::Url{"https://doyoumissraving.intheloop.bio/api/maps/" + texture["path"].get<std::string>()});
            if (imageResponse.status_code != 200) {
                std::cerr << "Error: " << imageResponse.status_code << std::endl;
                return;
            }

            std::ofstream file((paths::IMAGES_PATH / (texture["_id"].get<std::string>() + ".png")));
            file << imageResponse.text;
            file.close();
        }

        for (auto& sprite: data["sprites"]) {
            std::unique_ptr<engine::StaticSprite> spriteEntity = std::make_unique<engine::StaticSprite>(
                    container, labels,
                    (paths::IMAGES_PATH / (sprite["texture"].get<std::string>() + ".png")).string(),
                    math::Vec2<float>(sprite["x"], sprite["y"]),
                    math::Vec2<float>(sprite["width"], sprite["height"])
            );
            ((Map*)labels["map"])->addSprite(spriteEntity.get());
            addEntity(std::move(spriteEntity), sprite["name"]);
        }

        for (int i = 0; i < data["tiles"].size(); i++) {
            int x = i % (int) data["width"];
            int y = i / (int) data["width"];

            if (data["tiles"][i].find("floor") != data["tiles"][i].end())
                ((Map*)labels["map"])->getTile(x, y).changeTexture(
                        (paths::IMAGES_PATH / (data["tiles"][i]["floor"].get<std::string>() + ".png")).string()
                );
            if (data["tiles"][i].find("ceiling") != data["tiles"][i].end())
                ((Map*)labels["map"])->getTile(x, y).changeTexture(
                        (paths::IMAGES_PATH / (data["tiles"][i]["ceiling"].get<std::string>() + ".png")).string()
                );
            if (data["tiles"][i].find("texture") != data["tiles"][i].end())
                ((Map*)labels["map"])->getTile(x, y).changeTexture(
                        (paths::IMAGES_PATH / (data["tiles"][i]["texture"].get<std::string>() + ".png")).string()
                );

            ((Map*)labels["map"])->getTile(x, y).empty = data["tiles"][i]["empty"];
        }

        if (data.find("enemyStartX") != data.end()) {
            std::unique_ptr<engine::Entity> enemy = std::make_unique<engine::Enemy>(
                    container, labels,
                    (paths::IMAGES_PATH / "iosub gimnastica.png").string(),
                    math::Vec2<float>(data["enemyStartX"], data["enemyStartY"]),
                    math::Vec2<float>(constants::DEFAULT_TILESIZE - 10, constants::DEFAULT_TILESIZE - 10),
                    dynamic_cast<engine::Player&>(*labels.at("player")),
                    dynamic_cast<engine::Map&>(*labels.at("map"))
            );
            ((Map*)labels["map"])->addSprite((Sprite*) enemy.get());
            addEntity(std::move(enemy), "enemy");
        }

        std::unique_ptr<engine::Entity> raycaster = std::make_unique<Raycaster>(
                container, labels, playerRef, mapRef
        );
        addEntity(std::move(raycaster), "raycaster");
    }

    void EditorMapScene::update(float deltaTime) {
        Scene::update(deltaTime);
    }
} // engine