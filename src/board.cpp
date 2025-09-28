
#include <stdexcept>
#include <sstream>

#include "board.hpp"


Board::Board(unsigned int width, unsigned int height, Vector4 bounds)
    : width(width), height(height), bounds(bounds)
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


bool Board::checkForWin()
{
    //if (cachedWin) {
    //    return true;
    //}

    bool win = false;


    Cell *startCell;
    Cell *endCell;
    std::shared_ptr<Player> startPlayer;

    // Check vertical
    for (auto &column : cells) {
        startPlayer = nullptr;
        startCell = nullptr;
        endCell = nullptr;
        win = true;
        for (auto &cell : column) {
            if (cell.isPlayable()) {
                // Row is not completely filled.
                win = false;
                break;
            }

            auto thisPlayer = cell.getPlayer();
            if (startCell == nullptr) {
                startCell = &cell;
                startPlayer = thisPlayer;
                continue;
            }

            if (thisPlayer != startPlayer) {
                // A win requires a row of single player.
                // TODO, this won't work if board is not 3x3
                win = false;
                break;
            }
            endCell = &cell;
        }
        if (win) {
            cachedWin = true;

            winStart = startCell->getPosition();
            winEnd = endCell->getPosition();

            return true;
        }
    }

    // Check horizontal
    for (int y = 0; y < height; y++) {
        startPlayer = nullptr;
        startCell = nullptr;
        endCell = nullptr;
        win = true;
        for (int x = 0; x < width; x++) {
            Cell *cell = &cells[x][y];

            if (cell->isPlayable()) {
                // Row is not completely filled.
                win = false;
                break;
            }

            auto thisPlayer = cell->getPlayer();
            if (startCell == nullptr) {
                startCell = cell;
                startPlayer = thisPlayer;
                continue;
            }

            if (thisPlayer != startPlayer) {
                // A win requires a row of single player.
                // TODO, this won't work if board is not 3x3
                win = false;
                break;
            }
            endCell = cell;
        }
        if (win) {
            cachedWin = true;

            winStart = startCell->getPosition();
            winEnd = endCell->getPosition();

            return true;
        }
    }

    // Diagonal top-left -> bottom-right
    startPlayer = nullptr;
    startCell = nullptr;
    endCell = nullptr;
    win = true;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (x != y) {
                continue;
            }
            Cell *cell = &cells[x][y];

            if (cell->isPlayable()) {
                // Row is not completely filled.
                win = false;
                break;
            }

            auto thisPlayer = cell->getPlayer();
            if (startCell == nullptr) {
                startCell = cell;
                startPlayer = thisPlayer;
                continue;
            }

            if (thisPlayer != startPlayer) {
                // A win requires a row of single player.
                // TODO, this won't work if board is not 3x3
                win = false;
                break;
            }
            endCell = cell;
        }
    }
    if (win) {
        cachedWin = true;

        winStart = startCell->getPosition();
        winEnd = endCell->getPosition();

        return true;
    }

    // Diagonal top-right -> bottom-left
    startPlayer = nullptr;
    startCell = nullptr;
    endCell = nullptr;
    win = true;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (x != y) {
                continue;
            }
            int reversedX = width - (x + 1);
            Cell *cell = &cells[reversedX][y];

            if (cell->isPlayable()) {
                // Row is not completely filled.
                win = false;
                break;
            }

            auto thisPlayer = cell->getPlayer();
            if (startCell == nullptr) {
                startCell = cell;
                startPlayer = thisPlayer;
                continue;
            }

            if (thisPlayer != startPlayer) {
                // A win requires a row of single player.
                // TODO, this won't work if board is not 3x3
                win = false;
                break;
            }
            endCell = cell;
        }
    }
    if (win) {
        cachedWin = true;

        winStart = startCell->getPosition();
        winEnd = endCell->getPosition();

        return true;
    }

    return false;
}
