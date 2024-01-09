//
// Created by intheloop on 09.01.2024.
//

#include "Game.h"
#include "config.h"

Game::Game():
    window(
    sf::VideoMode(config::DEFAULT_WIDTH, config::DEFAULT_HEIGHT),
    "Do you miss raving?"
    ) {
}

void Game::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }

        window.clear();

        window.display(); // update
    }
}
