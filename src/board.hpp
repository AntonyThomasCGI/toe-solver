#pragma once

#include <utility>
#include <vector>

#include "raylib.h"
#include "cell.hpp"


class Board
{
public:
    Board(unsigned int width, unsigned int height, Vector4 bounds);
    ~Board();

    void drawGrid();
    void drawPlayers();

    void playInCell(std::shared_ptr<Player> player, unsigned int x, unsigned int y);

    std::vector<std::vector<Cell>> getCells() { return cells; }

    bool checkForWin(unsigned int winTarget);
    std::pair<Vector2, Vector2> getWinPositions();

private:
    unsigned int width;
    unsigned int height;
    Vector4 bounds;

    std::vector<std::vector<Cell>> cells;

    bool cachedWin = false;

    Vector2 winStart = {};
    Vector2 winEnd = {};
};
