//
// Created by Alex on 1/23/2024.
//

#ifndef RAYGAME_ASTAR_H
#define RAYGAME_ASTAR_H


#include <vector>
#include <queue>
#include <unordered_set>
#include <functional>
#include "../../utils/Math.h"
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
        void addToOpenSet(int x, int y, float g, const engine::math::Vec2<int> &target, HeuristicFunction heuristic, const Player& player);
    public:
        AStar() = default;
        ~AStar() = default;
        Player& player;


        bool isValidTile(int x, int y,const Player& player);
        engine::math::Vec2<float>
        findPath(const engine::math::Vec2<float> &start, const Player& player);
    };

} // game

#endif //RAYGAME_ASTAR_H