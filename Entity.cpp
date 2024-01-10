//
// Created by intheloop on 10.01.2024.
//

#include "Entity.h"

namespace engine {
    static ull generateId() {
        static ull id = 0;
        return id++;
    }

    Entity::Entity(): id(generateId()) {

    }

    ull Entity::getId() const {
        return id;
    }
} // engine