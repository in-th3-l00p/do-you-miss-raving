//
// Created by Alex on 1/23/2024.
//

#include "AStar.h"
#include "Player.h"


namespace game {

    void AStar::addToOpenSet(int x, int y, float g, const engine::math::Vec2<int>& target, HeuristicFunction heuristic, const Map& map) {
        if (closedSet.find(x * map.getWidth() + y) == closedSet.end()) {
            float h = heuristic(x, y, target);  // Calculate heuristic
            float f = g + h;
            openSet.push({x, y, f, g});
        }
    }

    bool AStar::isValidTile(int x, int y, const Map& map) {
        return x >= 0 && x < map.getWidth() && y >= 0 && y < map.getHeight() && map.getTiles()[y][x].empty;
    }

    engine::math::Vec2<float> AStar::findPath(const engine::math::Vec2<float>& start, const Map& map) {
        int tileSize = engine::constants::DEFAULT_TILESIZE;

        engine::math::Vec2<int> startTile = {
                static_cast<int>(start.x / tileSize),
                static_cast<int>(start.y / tileSize)
        };

        engine::math::Vec2<int> goalTile = {
                static_cast<int>(player.getPosition().x / tileSize),
                static_cast<int>(player.getPosition().y / tileSize)
        };

        auto heuristic = [tileSize](int x, int y, const engine::math::Vec2<int>& target) {
            return std::sqrt(static_cast<float>((x - target.x) * (x - target.x) + (y - target.y) * (y - target.y))) * tileSize;
        };

        auto calculateCost = [](int currentX, int currentY, int neighborX, int neighborY) {
            int dx = std::abs(neighborX - currentX);
            int dy = std::abs(neighborY - currentY);
            float diagonalCostMultiplier = 1.4f;
            float straightCost = 1.0f;
            float diagonalCost = straightCost * diagonalCostMultiplier;

            return (dx == 1 && dy == 1) ? diagonalCost : straightCost;
        };

        openSet.push({startTile.x, startTile.y, 0.0f, 0.0f});

        while (!openSet.empty()) {
            Node current = openSet.top();
            openSet.pop();

            if (current.x == goalTile.x && current.y == goalTile.y) {
                return engine::math::Vec2<float>(current.x * tileSize, current.y * tileSize);
            }

            // Process neighbors (adjacent tiles)
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    if (i == 0 && j == 0) continue;

                    int neighborX = current.x + i;
                    int neighborY = current.y + j;

                    if (isValidTile(neighborX, neighborY, map)) {
                        float newG = current.g + calculateCost(current.x, current.y, neighborX, neighborY);
                        addToOpenSet(neighborX, neighborY, newG, goalTile, heuristic, map);
                    }
                }
            }

            closedSet.insert(current.x * map.getWidth() + current.y);
        }
        return start;
    }

} // game