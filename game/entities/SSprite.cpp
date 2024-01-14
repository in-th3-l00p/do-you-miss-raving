//
// Created by Alex on 1/13/2024.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include "SSprite.h"

namespace game {
    SSprite::SSprite(
            bool inView,
            engine::math::Vec2<float> position,
            float enemySpeed,
            float direction,
            float distance,
            float numberddddd
    ) : inView(inView), position(position), enemySpeed(enemySpeed), direction(direction), distance(distance), number(number) {
        setZIndex(11);
        initTexture();
        initSprite();
    }

    sf::Texture texture;
    sf::Sprite sprite;

    void SSprite::initTexture(){
        if(!texture.loadFromFile("resources/images/capu ba.png")) {
            std::cerr << "Error loading image from file\n";
            return;
        }
    }

    void SSprite::initSprite() {
        sprite.setTexture(texture);
    }

    float SSprite::getNumber() const {
        return number;
    }

    void SSprite::setNumber(float number) {
        SSprite::number = number;
    }

    bool SSprite::isInView() const {
        return inView;
    }

    float SSprite::getEnemySpeed() const {
        return enemySpeed;
    }

    float SSprite::getDirection() const {
        return direction;
    }

    float SSprite::getDistance() const {
        return distance;
    }

    int SSprite::getHeight(sf::RenderWindow& window) const
    {
        return round(window.getSize().x / (distance * tan(engine::math::degToRad(0.5f * 90))));
    }

    int SSprite::getWidth(sf::RenderWindow& window) const
    {
        float sprite_height = sprite.getScale().y;
        float sprite_width = sprite.getScale().x;

        return round(window.getSize().y * sprite_width / (distance * sprite_height * tan(engine::math::degToRad(0.5f * 90))));
    }


    const engine::math::Vec2<float>& SSprite::getPosition() const {
        return position;
    }
//
    void SSprite::render(sf::RenderWindow& window)
    {
        float sprite_height = sprite.getScale().y;
        float sprite_width = sprite.getScale().x;

        std::cout << "DrawSprite position: " << position.x << ", " << position.y << "\n";

        inView &= window.getSize().y > number + position.y && window.getSize().x > position.x && position.x> -1 * getWidth(window) && number + position.y > -1 * getHeight(window);
        drawSprite(sf::Vector2<short>(position.x, position.y+number), window, getWidth(window)/sprite_width, getHeight(window)/sprite_height);
    }

    void SSprite::drawSprite(const sf::Vector2<short>& i_position, sf::RenderWindow& i_window, const float i_scale_x, const float i_scale_y) {

        sprite.setOrigin(sprite.getScale().x, sprite.getScale().y / 2);
        sprite.setPosition(sf::Vector2f(i_position));
        sprite.setScale(i_scale_x, i_scale_y);

        i_window.draw(sprite);
    }

    void SSprite::setPosition(const engine::math::Vec2<float> &position) {
        SSprite::position = position;
    }

    void SSprite::update(float delta) {
        position.x +=delta*1;
    }
} // game