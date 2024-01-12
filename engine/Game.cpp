//
// Created by intheloop on 09.01.2024.
//

#include "Game.h"
#include "../utils/Constants.h"

Game::Game():
    window(
    sf::VideoMode(utils::constants::DEFAULT_WIDTH, utils::constants::DEFAULT_HEIGHT),
    "Do you miss raving?"
    ) {
    scene = std::unique_ptr<utils::Scene>(new utils::TestScene(window, scene));
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
