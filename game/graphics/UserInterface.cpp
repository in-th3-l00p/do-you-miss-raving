//
// Created by intheloop on 12.01.2024.
//

#include "UserInterface.h"
#include "../../utils/Constants.h"

namespace game {
    namespace ui {
        sf::Font &FontLoader::getFont(const std::string &fontName) {
            if (fontsContainer.find(fontName) == fontsContainer.end()) {
                sf::Font font;
                font.loadFromFile(fontName);
                fontsContainer[fontName] = font;
            }
            return fontsContainer[fontName];
        }

        sf::Font &FontLoader::getDefault() {
            return getFont(constants::DEFAULT_FONT);
        }

        MenuScene::MenuScene(
                sf::RenderWindow &window,
                std::unique_ptr<Scene> &sceneRef
        ) : Scene(window, sceneRef) {

        }

        void MenuScene::update(float deltaTime) {
            Scene::update(deltaTime);
        }
    } // ui
} // game