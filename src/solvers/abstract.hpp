#pragma once

#include <chrono>
#include <iostream>
#include <thread>
#include <utility>

#include "../board.hpp"
#include "../datatypes.hpp"



/**
 * AbstractSolver describes the interface to implement a solver.
 * To create a new solver inherit from this class and define a `solve` method.
 */
class AbstractSolver
{
public:

    /** Wrapper around solve method to provide timing log information. */
    Coordinate runSolve(std::shared_ptr<Board> board) {
        auto start = std::chrono::high_resolution_clock::now();

        auto result = solve(board);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "INFO: Solver calculated move (" << result.x << ", "
             << result.y << ") in " << duration.count() << "ms" << std::endl;

        return result;
    }

private:

    /** solve takes a board as input and returns a move. */
    virtual Coordinate solve(std::shared_ptr<Board> board) = 0;
};
