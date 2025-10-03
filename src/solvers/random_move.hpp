#pragma once

#include <chrono>
#include <random>
#include <thread>
#include <utility>

#include "../board.hpp"


typedef std::pair<int, int> Move;


// Create a random move.
class RandomMoveSolver
{
public:

    static Move solve(std::shared_ptr<Board> board) {
        // Simulate big think
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> distWidth(0, board->getWidth());
        std::uniform_int_distribution<std::mt19937::result_type> distHeight(0, board->getWidth());

        int x = distWidth(rng);
        int y = distHeight(rng);
        while (!board->canPlayInCell(x, y)) {
            x = distWidth(rng);
            y = distHeight(rng);
        }

        return {x, y};
    };

};
