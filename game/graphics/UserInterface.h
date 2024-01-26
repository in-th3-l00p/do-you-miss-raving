//
// Created by intheloop on 12.01.2024.
//

#ifndef RAYGAME_USERINTERFACE_H
#define RAYGAME_USERINTERFACE_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "../../engine/Entity.h"
#include "../../engine/Scene.h"
#include "../../utils/Constants.h"
#include "../../utils/Paths.h"

namespace engine::ui {
    // singleton class that loads fonts automatically and stores them once they are loaded
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

    inline FontLoader* FontLoader::instance = nullptr;

    class Button: public engine::Entity {
    private:
        sf::RectangleShape shape;
        sf::Text text;

        sf::Color backgroundColor;
        sf::Color textColor;

        void initText(
                const std::string& text,
                const sf::Font& font,
                unsigned int characterSize,
                const sf::Color& color
                );

    public:
        Button(
                const std::set<std::unique_ptr<Entity>>& container,
                const std::map<std::string, Entity*>& labels,
                float x, float y,
                float width, float height,
                const std::string& text,
                const sf::Font& font,
                unsigned int characterSize,
                const sf::Color& backgroundColor,
                const sf::Color& textColor
                );

        void render(sf::RenderWindow& window) override;
        void update(float deltaTime) override;

        bool isPressed(const sf::Vector2f& mousePos) const;
    };

    class MenuScene: public engine::Scene {
    private:
        float menuWidth = 500.f;
        float menuHeight = 300.f;
        float margin = 10.f;

        sf::Texture logo;

    public:
        explicit MenuScene(
                sf::RenderWindow &window,
                std::unique_ptr<Scene>& sceneRef
                );

        void update(float deltaTime) override;
    };
}

#endif //RAYGAME_USERINTERFACE_H
