//
// Created by intheloop on 12.01.2024.
//

#include <filesystem>
#include <iostream>
#include "UserInterface.h"
#include "../../engine/Scene.h"

namespace engine::ui {
    // font loader implementation
    sf::Font &FontLoader::getFont(const std::string &fontName) {
        if (fontsContainer.find(fontName) == fontsContainer.end()) {
            sf::Font font;
            engine::paths::fs::path fontPath = engine::paths::FONTS_DIR / fontName;
            font.loadFromFile(fontPath.string());
            fontsContainer[fontName] = font;
        }
        return fontsContainer[fontName];
    }

    sf::Font &FontLoader::getDefault() {
        return getFont(engine::constants::DEFAULT_FONT);
    }

    // button implementation
    void Button::initText(
            const std::string &text,
            const sf::Font &font,
            unsigned int characterSize,
            const sf::Color &color
            ) {
        this->text.setFont(font);
        this->text.setString(text);
        this->text.setCharacterSize(characterSize);
        this->text.setFillColor(color);

        sf::FloatRect textRect = this->text.getLocalBounds();
        this->text.setOrigin(textRect.left + textRect.width / 2.0f,
                             textRect.top + textRect.height / 2.0f);
        this->text.setPosition(shape.getPosition().x + shape.getSize().x / 2.0f,
                               shape.getPosition().y + shape.getSize().y / 2.0f);
    }

    Button::Button(
            const std::set<std::unique_ptr<Entity>>& container,
            const std::map<std::string, Entity*>& labels,
            float x, float y,
            float width, float height,
            const std::string &text,
            const sf::Font &font,
            unsigned int characterSize,
            const sf::Color &backgroundColor,
            const sf::Color &textColor
            ):
            Entity(container, labels),
            backgroundColor(backgroundColor),
            textColor(textColor) {
        shape.setPosition(x, y);
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(backgroundColor);

        initText(text, font, characterSize, textColor);

        setZIndex(1);
        sf::FloatRect textRect = this->text.getLocalBounds();
        this->text.setOrigin(textRect.left + textRect.width / 2.0f,
                             textRect.top + textRect.height / 2.0f);
        this->text.setPosition(shape.getPosition().x + shape.getSize().x / 2.0f,
                               shape.getPosition().y + shape.getSize().y / 2.0f);
    }

    void Button::render(sf::RenderWindow &window) {
        window.draw(shape);
        window.draw(text);
    }

    void Button::update(float deltaTime) {
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top + textRect.height / 2.0f);
        text.setPosition(shape.getPosition().x + shape.getSize().x / 2.0f,
                         shape.getPosition().y + shape.getSize().y / 2.0f);
    }

    bool Button::isPressed(const sf::Vector2f& mousePos) const {
        return
                sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
                shape.getGlobalBounds().contains(mousePos);
    }


    // menu scene implementation
    MenuScene::MenuScene(
            sf::RenderWindow &window,
            std::unique_ptr<Scene> &sceneRef
    ) : Scene(window, sceneRef) {
        std::unique_ptr<engine::Entity> button = std::make_unique<Button>(
                container, labels,
                100, 100, 100, 50,
                "Start",
                FontLoader::getInstance()->getDefault(),
                24,
                sf::Color::Blue,
                sf::Color::White
                );
        startButton = dynamic_cast<Button*>(button.get());
        addEntity(std::move(button));
    }

    void MenuScene::update(float deltaTime) {
        Scene::update(deltaTime);

        const sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
        if (startButton->isPressed(mousePos)) {
            sceneRef = std::make_unique<engine::TestScene>(window, sceneRef);
        }
    }

}