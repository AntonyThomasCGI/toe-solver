
#pragma once

#include "abstract.hpp"


/**
 * PlayCloseSolver attempts to find the move that would be adjacent to the most amount of opponent tiles as possible.
 */
class PlayCloseSolver : public AbstractSolver
{
private:

    Coordinate solve(std::shared_ptr<Board> board) {

        auto cells = board->getCells();

        // All possible directions to find nearby neighbors.
        std::vector<std::pair<int, int>> directions = {
            {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}
        };

        int bestNumNeighbors = -1;
        Cell *bestCell = nullptr;

        for (auto &column : cells) {
            for (auto &cell : column) {
                // Don't consider unplayable cells.
                if (!cell.isPlayable()) {
                    continue;
                }

                Coordinate coord = cell.getCoordinate();

                int numNeighbors = 0;
                int neighborX;
                int neighborY;
                for (auto &[directionX, directionY] : directions) {
                    neighborX = coord.x + directionX;
                    if (neighborX < 0 || neighborX >= board->getWidth()) {
                        continue;
                    }
                    neighborY = coord.y + directionY;
                    if (neighborY < 0 || neighborY >= board->getHeight()) {
                        continue;
                    }

                    auto neighbor = cells[neighborX][neighborY].getPlayer();
                    if (neighbor == nullptr) {
                        // No one played here.
                        continue;
                    }

                    // TODO, hard coding users player to player0 for now.
                    if (neighbor->getId() == "player0") {
                        numNeighbors ++;
                    }
                }
                if (numNeighbors > bestNumNeighbors) {
                    bestNumNeighbors = numNeighbors;
                    bestCell = &cell;
                }
            }
        }

        if (bestCell == nullptr) {
            throw std::runtime_error("Could not determine a bestCell after evaluating all cells");
        }

        return bestCell->getCoordinate();
    };
};
