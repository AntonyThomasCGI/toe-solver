
#pragma once

#include <cmath>
#include <vector>

#include "abstract.hpp"
#include "random_move.hpp"


/**
 * BlockConnectionsSolver tries its best to block opponents connections by finding the spot that blocks
 * the longest chain from the opponent. If there are two moves which block an opponents connection of
 * equal length, choose the move closest to the middle of the board.
 */
class BlockConnectionsSolver : public AbstractSolver
{
public:

    Coordinate solve(std::shared_ptr<Board> board, unsigned int winTarget) {

        auto cells = board->getCells();

        // All possible axes to find neighbor connections (each vector component
        // is multiplied by -1 to traverse in the opposite direction).
        std::vector<Coordinate> axes = {
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
                    // Count the number of neighbors-in-a-row that contain opponent player
                    // and keep track of the open cells to see if a win is even possible.
                    int neighborInARow = 0;
                    int openCount = 1;  // Includes current cell so start at 1.
                    bool foundOpen = false;

                    // Traverse in both directions along this axis.
                    std::vector<int> stack = {1, -1};

                    int count = 1;  // Keep track of n-cells from the cell we are evaluating.
                    while (!stack.empty()) {
                        int directionMult = stack.back();
                        int directionX = axis.x * directionMult;
                        int directionY = axis.y * directionMult;
                        int neighborX = coord.x + directionX * count;
                        int neighborY = coord.y + directionY * count;
                        if (
                            (neighborX < 0 || neighborX >= board->getWidth()) ||
                            (neighborY < 0 || neighborY >= board->getHeight())
                        ) {
                            // Out of bounds.
                            stack.pop_back();
                            count = 1;
                            foundOpen = false;
                            continue;
                        }

                        auto neighbor = cells[neighborX][neighborY].getPlayer();
                        if (neighbor == nullptr) {
                            // No one played here.
                            openCount++;
                            foundOpen = true;
                        }
                        // TODO, hard coding users player to player0 for now.
                        else if (neighbor->getId() == "player0") {
                            if (!foundOpen) {
                                neighborInARow++;
                            }
                            openCount++;
                        } else {
                            stack.pop_back();
                            count = 1;
                            foundOpen = false;
                            continue;
                        }
                        count++;
                    }

                    // Take max since moving diagonal counts as 1 move.
                    int distanceToOrigin = std::max(
                        abs(std::floor(board->getWidth() / 2) - coord.x),
                        abs(std::floor(board->getHeight() / 2) - coord.y));

                    // Check if we have a better neighbor-in-a-row count and it's possible for the opponent to
                    // win on this connection. Or if it is equal to the existing best, use the coordinate that
                    // is closer to center of the board.
                    if (
                        openCount >= winTarget &&
                        (
                            neighborInARow > bestNeighborInARow ||
                            (
                                neighborInARow == bestNeighborInARow &&
                                distanceToOrigin < bestDistanceToOrigin
                            )
                        )
                    ) {
                        bestNeighborInARow = neighborInARow;
                        bestCoordinate = coord;
                        bestDistanceToOrigin = distanceToOrigin;
                    }
                }
            }
        }

        if (bestCoordinate.x == -1 && bestCoordinate.y == -1) {
            // Opponent has no connections that can win the game, just play a random move.
            auto randomSolver = RandomMoveSolver();
            bestCoordinate = randomSolver.solve(board, winTarget);
        }

        return bestCoordinate;
    }
};
