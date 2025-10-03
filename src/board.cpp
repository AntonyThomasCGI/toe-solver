
#include <iostream>
#include <stdexcept>
#include <sstream>

#include "board.hpp"


Board::Board(unsigned int width, unsigned int height, Vector4 bounds)
    : width(static_cast<int>(width)), height(static_cast<int>(height)), bounds(bounds)
{
    float cellSizeX = (bounds.z - bounds.x) / width;
    float cellSizeY = (bounds.w - bounds.y) / height;

    cells.resize(width);

    for (int i = 0; i < width; i++) {
        float xPos = i * cellSizeX + bounds.x;

        for (int j = 0; j < height; j++) {
            float yPos = j * cellSizeY + bounds.y;

            Rectangle cellRect = {xPos, yPos, cellSizeX, cellSizeY};
            Cell cell = Cell(cellRect);

            cells[i].push_back(cell);
        }
    }
}


Board::~Board()
{

}


/**
 * Draw the board grid within given bounds.
 */
void Board::drawGrid()
{
    float xCellSize = (bounds.z - bounds.x) / width;
    float yCellSize = (bounds.w - bounds.y) / height;

    for (int i = 1; i < width; i++) {
        float posX = i * xCellSize + bounds.x;

        DrawLineEx({posX, bounds.y}, {posX, bounds.w}, 3.0, BLACK);
    }

    for (int i = 1; i < height; i++) {
        float posY = i * yCellSize + bounds.y;

        DrawLineEx({bounds.x, posY}, {bounds.z, posY}, 3.0, BLACK);
    }
}

void Board::drawPlayers()
{
    for (std::vector<Cell>& cellCol : cells) {
        for (Cell& cell : cellCol) {
            if (cell.isPlayable()) {
                continue;  // No player.
            }

            Rectangle rect = cell.getRect();
            Vector2 pos = cell.getPosition();

            std::shared_ptr<Player> player = cell.getPlayer();

            player->drawPlayer(rect.width * 0.6, pos);
        }
    }
}

bool Board::canPlayInCell(unsigned int x, unsigned int y)
{
    if (width <= x || height <= y) {
        std::stringstream ss;
        ss << "WARNING: canPlayInCell received out of bounds coordinate: " << x << ", " << y;
        std::cout << ss.str() << std::endl;
        return false;
    }

    return cells[x][y].isPlayable();
}


void Board::playInCell(std::shared_ptr<Player> player, unsigned int x, unsigned int y)
{
    if (width <= x || height <= y) {
        std::stringstream ss;
        ss << "playInCell received out of bounds coordinate: " << x << ", " << y;
        throw std::runtime_error(ss.str());
    }

    cells[x][y].setPlayed(player);
}


std::pair<Vector2, Vector2> Board::getWinPositions()
{
    if (!cachedWin) {
        throw std::runtime_error("Tried to access win positions but game is not won!");
    }
    return {winStart, winEnd};
}


bool Board::checkForWin(unsigned int winTarget)
{
    if (cachedWin) {
        return true;
    }

    bool win = false;

    std::shared_ptr<Player> startPlayer;
    Cell *startCell;
    unsigned int winCount;

    // Check columns
    for (auto &column : cells) {
        startPlayer = nullptr;
        startCell = nullptr;
        winCount = 0;

        for (auto &cell : column) {
            if (cell.isPlayable()) {
                // Cell has not been played in yet, reset count.
                startPlayer = nullptr;
                startCell = nullptr;
                winCount = 0;
                continue;
            }

            auto thisPlayer = cell.getPlayer();
            if (startCell == nullptr) {
                // Found the first legit move in this column,
                // start count from this player.
                startCell = &cell;
                startPlayer = thisPlayer;
                winCount = 1;
            } else if (thisPlayer != startPlayer) {
                // Player changed, start count again from this player.
                startPlayer = thisPlayer;
                startCell = &cell;
                winCount = 1;
            } else {
                // Same player, increment count.
                winCount++;
            }

            // And finally, check if someone has won.
            if (winCount == winTarget) {
                winStart = startCell->getPosition();
                winEnd = cell.getPosition();
                cachedWin = true;
                return true;
            }
        }
    }

    // Check rows
    for (int y = 0; y < height; y++) {
        startPlayer = nullptr;
        startCell = nullptr;
        winCount = 0;

        for (int x = 0; x < width; x++) {
            Cell &cell = cells[x][y];

            if (cell.isPlayable()) {
                // Cell has not been played in yet, reset count.
                startPlayer = nullptr;
                startCell = nullptr;
                winCount = 0;
                continue;
            }

            auto thisPlayer = cell.getPlayer();
            if (startCell == nullptr) {
                // Found the first legit move in this row,
                // start count from this player.
                startCell = &cell;
                startPlayer = thisPlayer;
                winCount = 1;
            } else if (thisPlayer != startPlayer) {
                // Player changed, start count again from this player.
                startPlayer = thisPlayer;
                startCell = &cell;
                winCount = 1;
            } else {
                // Same player, increment count.
                winCount++;
            }

            // And finally, check if someone has won.
            if (winCount == winTarget) {
                winStart = startCell->getPosition();
                winEnd = cell.getPosition();
                cachedWin = true;
                return true;
            }
        }
    }

    // Check diagonal top-left -> bottom-right
    // Diagonals are grouped by (row - column) = 0, 1, -1, 2, -2 ...
    // but iterate from negative -> positive
    int numRedundantCells = static_cast<int>(winTarget);
    for (int d = -(height - numRedundantCells); d <= (width - numRedundantCells); d++) {
        startPlayer = nullptr;
        startCell = nullptr;
        winCount = 0;

        for (int x = 0; x < width; x++) {

            int y = x - d;
            if (y < 0 || y >= height) {
                // Out of bounds.
                continue;
            }

            Cell &cell = cells[x][y];

            if (cell.isPlayable()) {
                // Cell has not been played in yet, reset count.
                startPlayer = nullptr;
                startCell = nullptr;
                winCount = 0;
                continue;
            }

            auto thisPlayer = cell.getPlayer();
            if (startCell == nullptr) {
                // Found the first legit move in this diagonal,
                // start count from this player.
                startCell = &cell;
                startPlayer = thisPlayer;
                winCount = 1;
            } else if (thisPlayer != startPlayer) {
                // Player changed, start count again from this player.
                startPlayer = thisPlayer;
                startCell = &cell;
                winCount = 1;
            } else {
                // Same player, increment count.
                winCount++;
            }

            // And finally, check if someone has won.
            if (winCount == winTarget) {
                winStart = startCell->getPosition();
                winEnd = cell.getPosition();
                cachedWin = true;
                return true;
            }
        }
    }

    // Check diagonal top-right -> bottom-left
    // Diagonals are grouped by (row + column) = 0, 1, 2, 3 ...
    //numRedundantCells = static_cast<int>(winTarget) - 1;
    for (int s = numRedundantCells - 1; s < width + height - numRedundantCells; s++) {
        startPlayer = nullptr;
        startCell = nullptr;
        winCount = 0;

        for (int x = 0; x < width; x++) {

            int y = s - x;
            if (y < 0 || y >= height) {
                // Out of bounds.
                continue;
            }

            Cell &cell = cells[x][y];

            if (cell.isPlayable()) {
                // Cell has not been played in yet, reset count.
                startPlayer = nullptr;
                startCell = nullptr;
                winCount = 0;
                continue;
            }

            auto thisPlayer = cell.getPlayer();
            if (startCell == nullptr) {
                // Found the first legit move in this diagonal,
                // start count from this player.
                startCell = &cell;
                startPlayer = thisPlayer;
                winCount = 1;
            } else if (thisPlayer != startPlayer) {
                // Player changed, start count again from this player.
                startPlayer = thisPlayer;
                startCell = &cell;
                winCount = 1;
            } else {
                // Same player, increment count.
                winCount++;
            }

            // And finally, check if someone has won.
            if (winCount == winTarget) {
                winStart = startCell->getPosition();
                winEnd = cell.getPosition();
                cachedWin = true;
                return true;
            }
        }
    }

    return false;
}
