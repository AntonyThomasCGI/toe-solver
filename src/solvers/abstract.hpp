#pragma once

#include <utility>

#include "../board.hpp"


typedef std::pair<int, int> Move;



/**
 * AbstractSolver describes the interface to implement a solver.
 * To create a new solver inherit from this class and define a solve method.
 */
class AbstractSolver
{
public:

    /** solve takes a board as input and returns a move. */
    virtual Move solve(std::shared_ptr<Board> board) = 0;

    // TODO, add move validation convenience methods?
};
