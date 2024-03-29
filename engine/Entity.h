//
// Created by intheloop on 10.01.2024.
//

#ifndef RAYGAME_ENTITY_H
#define RAYGAME_ENTITY_H

#include <set>
#include <memory>
#include <map>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../utils/Constants.h"

namespace engine {
    class Entity {
    private:
        ull id;
        ull zIndex = 0;

    protected:
        const std::set<std::unique_ptr<Entity>>& container;
        const std::map<std::string, Entity*>& labels;

    public:
        Entity(
                const std::set<std::unique_ptr<Entity>>& container,
                const std::map<std::string, Entity*>& labels
                );
        virtual ~Entity() = default;

        virtual void render(sf::RenderWindow& window) = 0;
        virtual void update(float deltaTime) = 0;

        [[nodiscard]] ull getId() const;
        bool operator<(const Entity& other) const {
            return id < other.id;
        }

        ull getZIndex() const;

        void setZIndex(ull zIndex);
    };

} // engine

#endif //RAYGAME_ENTITY_H
