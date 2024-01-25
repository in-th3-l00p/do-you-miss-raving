//
// Created by intheloop on 10.01.2024.
//

#include <memory>
#include "Entity.h"

namespace engine {
    static ull generateId() {
        static ull id = 0;
        return id++;
    }

    Entity::Entity(
            const std::set<std::unique_ptr<Entity>>& container,
            const std::map<std::string, Entity*>& labels
        ) : id(generateId()), container(container), labels(labels) {

    }

    ull Entity::getId() const {
        return id;
    }

    ull Entity::getZIndex() const {
        return zIndex;
    }

    void Entity::setZIndex(ull zIndex) {
        Entity::zIndex = zIndex;
    }
} // engine