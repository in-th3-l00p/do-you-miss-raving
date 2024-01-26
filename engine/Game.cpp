//
// Created by intheloop on 09.01.2024.
//

#include <stdexcept>
#include "Game.h"
#include "../game/graphics/UserInterface.h"
#include "../game/entities/Player.h"
#include "imgui.h"
#include "imgui-SFML.h"

Game::Game():
    window(
    sf::VideoMode(engine::constants::DEFAULT_WIDTH, engine::constants::DEFAULT_HEIGHT),
    "Do you miss raving?"
    ) {
    if (!ImGui::SFML::Init(window))
        throw std::runtime_error("Failed to initialize ImGui");
    scene = std::unique_ptr<engine::Scene>(new engine::ui::MenuScene(window, scene));
}

void Game::run() {
    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                    window.setView(sf::View(visibleArea));
                    break;
            }
        }

        sf::Time dt = deltaClock.restart();
        ImGui::SFML::Update(window, dt);

        window.clear();
        scene->update(dt.asSeconds());
        ImGui::SFML::Render(window);
        window.display(); // update
    }
}
