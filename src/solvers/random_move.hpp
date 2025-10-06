#pragma once

#include <chrono>
#include <random>
#include <thread>
#include <utility>

#include "abstract.hpp"


/**
 * RandomMoveSolver picks a move at random. Careful of infinite monkey theorem, it still might
 * play a perfect game.
 */
class RandomMoveSolver : public AbstractSolver
{
private:

    Coordinate solve(std::shared_ptr<Board> board) override {
        // Simulate big think
        //std::this_thread::sleep_for(std::chrono::milliseconds(200));

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> distWidth(0, board->getWidth() - 1);
        std::uniform_int_distribution<std::mt19937::result_type> distHeight(0, board->getWidth() - 1);

        int x = distWidth(rng);
        int y = distHeight(rng);
        while (!board->canPlayInCell(x, y)) {
            x = distWidth(rng);
            y = distHeight(rng);
        }

        return {x, y};
    };

};
