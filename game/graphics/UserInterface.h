//
// Created by intheloop on 12.01.2024.
//

#ifndef RAYGAME_USERINTERFACE_H
#define RAYGAME_USERINTERFACE_H

#include <SFML/Graphics.hpp>
#include "../../engine/Entity.h"
#include "../../engine/Scene.h"

namespace game::ui {
    class FontLoader {
    private:
        static FontLoader* instance;
        FontLoader() = default;

        std::unordered_map<std::string, sf::Font> fontsContainer;

    public:
        FontLoader(const FontLoader&) = delete;
        FontLoader& operator=(const FontLoader&) = delete;
        static FontLoader* getInstance() {
            if (instance == nullptr)
                instance = new FontLoader();
            return instance;
        }

        sf::Font& getFont(const std::string& fontName);
        sf::Font& getDefault();
    };

    class MenuScene: public utils::Scene {
    public:
        explicit MenuScene(
                sf::RenderWindow &window,
                std::unique_ptr<Scene>& sceneRef
                );

        void update(float deltaTime) override;
    };
}

#endif //RAYGAME_USERINTERFACE_H
