//
// Created by intheloop on 09.01.2024.
//

#include "Game.h"
#include "../utils/Constants.h"
#include "../game/graphics/UserInterface.h"

Game::Game():
    window(
    sf::VideoMode(engine::constants::DEFAULT_WIDTH, engine::constants::DEFAULT_HEIGHT),
    "Do you miss raving?"
    ) {
    scene = std::unique_ptr<engine::Scene>(new game::ui::MenuScene(window, scene));
}

void Game::run() {
    sf::Clock deltaClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
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
        window.clear();
        scene->update(dt.asSeconds());
        window.display(); // update
    }
}
