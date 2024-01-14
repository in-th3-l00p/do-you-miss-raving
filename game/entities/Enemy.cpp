//
// Created by Alex on 1/13/2024.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Enemy.h"

namespace game {
    Enemy::Enemy(
            bool inView,
            engine::math::Vec2<float> position,
            float enemySpeed,
            float direction,
            float distance,
            float number,
            sf::Texture texture,
            sf::Sprite sprite
    ) : inView(inView), position(position), enemySpeed(enemySpeed), direction(direction), distance(distance), number(number),texture(texture),sprite(sprite) {
        setZIndex(1);
    }

    void Enemy::initTexture(){
        if(!texture.loadFromFile("resources/images/capu ba.png.png")) {
            std::cerr << "Error loading image from file\n";
            return;
        }
    }

    void Enemy::initSprite(){
        sf::Sprite enemySprite(texture);
    }

    float Enemy::getNumber() const {
        return number;
    }

    void Enemy::setNumber(float number) {
        Enemy::number = number;
    }

    bool Enemy::isInView() const {
        return inView;
    }

    float Enemy::getEnemySpeed() const {
        return enemySpeed;
    }

    float Enemy::getDirection() const {
        return direction;
    }

    float Enemy::getDistance() const {
        return distance;
    }

    int Enemy::getHeight(sf::RenderWindow& window) const
    {
        return round(window.getSize().x / (distance * tan(engine::math::degToRad(0.5f * 90))));
    }

    int Enemy::getWidth(sf::RenderWindow& window) const
    {
        float sprite_height = sprite.getScale().y;
        float sprite_width = sprite.getScale().x;

        return round(window.getSize().y * sprite_width / (distance * sprite_height * tan(engine::math::degToRad(0.5f * 90))));
    }


    const engine::math::Vec2<float>& Enemy::getPosition() const {
        return position;
    }
//
    void Enemy::render(sf::RenderWindow& window)
    {
        float sprite_height = sprite.getScale().y;
        float sprite_width = sprite.getScale().x;

        inView &= window.getSize().y > number + position.y && window.getSize().x > position.x && position.x> -1 * getWidth(window) && number + position.y > -1 * getHeight(window);
        drawSprite(sf::Vector2<short>(position.x, position.y+number), window, getWidth(window)/sprite_width, getHeight(window)/sprite_height);
    }

    void Enemy::drawSprite(const sf::Vector2<short>& i_position, sf::RenderWindow& i_window, const float i_scale_x, const float i_scale_y)
    {
        sprite.setOrigin(sprite.getScale().x, sprite.getScale().y /2);
        sprite.setPosition(sf::Vector2f(i_position));
        sprite.setScale(i_scale_x, i_scale_y);
        i_window.draw(sprite);
    }

    void Enemy::setPosition(const engine::math::Vec2<float> &position) {
        Enemy::position = position;
    }

    void Enemy::update(float delta) {
        position += direction*delta;
    }
} // game