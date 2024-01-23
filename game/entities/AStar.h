//
// Created by Alex on 1/23/2024.
//

#ifndef RAYGAME_ASTAR_H
#define RAYGAME_ASTAR_H


#include <vector>
#include <queue>
#include <unordered_set>
#include "../../utils/Math.h"
#include "Map.h"
#include "Player.h"

namespace game {

    class AStar {
    private:
        struct Node {
            int x, y;
            float f, g;

            bool operator>(const Node& other) const {
                return f > other.f;
            }
        };

        using HeuristicFunction = std::function<float(int, int, const engine::math::Vec2<int>&)>;

        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
        std::unordered_set<int> closedSet;

        void addToOpenSet(int x, int y, float g, const engine::math::Vec2<int>& target, HeuristicFunction heuristic);
        bool isValidTile(int x, int y);

    public:
        AStar() = default;
        ~AStar() = default;
        Player player;

        void
        addToOpenSet(int x, int y, float g, const engine::math::Vec2<int> &target, HeuristicFunction heuristic,
                     const Map &map);

        bool isValidTile(int x, int y, const Map &map);

        engine::math::Vec2<float>
        findPath(const engine::math::Vec2<float> &start, const Map &map);
    };

} // game

#endif //RAYGAME_ASTAR_H