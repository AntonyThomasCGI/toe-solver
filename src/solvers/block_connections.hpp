
#pragma once

#include <cmath>
#include <vector>

#include "abstract.hpp"


/**
 * BlockConnectionsSolver tries its best to block opponents connections by finding the spot that blocks
 * the longest chain from the opponent. If there are two moves which block an opponents connection of
 * equal length, choose the move closest to the middle of the board.
 */
class BlockConnectionsSolver : public AbstractSolver
{
private:

    Coordinate solve(std::shared_ptr<Board> board) {

        auto cells = board->getCells();

        // All possible axes to find neighbor connections (each vector component
        // is multiplied by -1 to traverse in the opposite direction).
        std::vector<std::pair<int, int>> axes = {
            {0, 1}, {1, 0}, {1, 1}, {1, -1}
        };

        // Keep track of the best coordinate and number of neighbors in a row.
        int bestNeighborInARow = -1;
        Coordinate bestCoordinate = {-1, -1};
        int bestDistanceToOrigin = 1000000000;

        for (auto &column : cells) {
            for (auto &cell : column) {
                // Don't consider unplayable cells.
                if (!cell.isPlayable()) {
                    continue;
                }

                Coordinate coord = cell.getCoordinate();

                for (auto &axis : axes) {
                    // Count the number of neighbors-in-a-row that contain opponent player.
                    int neighborInARow = 0;
                    int count = 1;

                    // Traverse in both directions along this axis.
                    std::vector<int> stack = {1, -1};

                    while (!stack.empty()) {
                        int directionMult = stack.back();
                        int directionX = axis.first * directionMult;
                        int directionY = axis.second * directionMult;

                        int neighborX = coord.x + directionX * count;
                        if (neighborX < 0 || neighborX >= board->getWidth()) {
                            // Out of bounds.
                            stack.pop_back();
                            count = 1;
                            continue;
                        }
                        int neighborY = coord.y + directionY * count;
                        if (neighborY < 0 || neighborY >= board->getHeight()) {
                            // Out of bounds.
                            stack.pop_back();
                            count = 1;
                            continue;
                        }

                        auto neighbor = cells[neighborX][neighborY].getPlayer();
                        if (neighbor == nullptr) {
                            // No one played here.
                            stack.pop_back();
                            count = 1;
                            continue;
                        }
                        // TODO, hard coding users player to player0 for now.
                        if (neighbor->getId() == "player0") {
                            neighborInARow++;
                        } else {
                            stack.pop_back();
                            count = 1;
                            continue;
                        }
                        count++;
                    }

                    int distanceToOrigin = abs(std::floor(board->getWidth() / 2) - coord.x)
                                         + abs(std::floor(board->getHeight() / 2) - coord.y);

                    // Check if we have a better neighbor-in-a-row count. Or if it is equal to the existng best,
                    // use the coordinate that is closer to center of the board.
                    if (
                        neighborInARow > bestNeighborInARow ||
                        (neighborInARow == bestNeighborInARow && distanceToOrigin < bestDistanceToOrigin)
                    ) {
                        bestNeighborInARow = neighborInARow;
                        bestCoordinate = coord;
                        bestDistanceToOrigin = distanceToOrigin;
                    }
                }
            }
        }

        return bestCoordinate;
    }
};
