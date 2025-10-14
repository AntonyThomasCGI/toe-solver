
#include <future>
#include <iostream>
#include <memory>
#include <thread>

#include "raylib.h"

#include "board.hpp"
#include "player.hpp"
#include "solvers/random_move.hpp"
#include "solvers/play_close.hpp"
#include "solvers/block_connections.hpp"
#include "datatypes.hpp"



// Window size
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;


// Game rules
const unsigned int EXTERNAL_GRID_WIDTH = 5;
const unsigned int EXTERNAL_GRID_HEIGHT = 5;

const unsigned int PLAYER_COUNT = 2;

const unsigned int WIN_TARGET = 4;

// Vs. bot?
const bool USE_SOLVER = true;

using SOLVER_CLASS = BlockConnectionsSolver;


// GUI sizing
const unsigned int BOARDER_PADDING = 20;


void run() {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Toe Solver");

    int gameBoardWidth = SCREEN_WIDTH - BOARDER_PADDING * 2;
    int gameBoardHeight = (gameBoardWidth / EXTERNAL_GRID_WIDTH) * EXTERNAL_GRID_HEIGHT;
    if ((gameBoardHeight + BOARDER_PADDING * 2) > SCREEN_HEIGHT) {
        gameBoardHeight = SCREEN_HEIGHT - BOARDER_PADDING * 2;
        gameBoardWidth = (gameBoardHeight / EXTERNAL_GRID_HEIGHT) * EXTERNAL_GRID_WIDTH;
    }

    float boardStartX = (SCREEN_WIDTH - gameBoardWidth) / 2;
    float boardStartY = BOARDER_PADDING;

    float boardEndX = boardStartX + gameBoardWidth;
    float boardEndY = boardStartY + gameBoardHeight;

    Vector4 boardBounds = {boardStartX, boardStartY, boardEndX, boardEndY};
    std::shared_ptr<Board> board = std::make_shared<Board>(
        EXTERNAL_GRID_WIDTH, EXTERNAL_GRID_HEIGHT, boardBounds
    );

    std::shared_ptr<Player> player0 = std::make_shared<Player>("player0", CROSS, MAROON);
    std::shared_ptr<Player> player1 = std::make_shared<Player>("player1", NAUGHT, DARKGREEN);

    auto activePlayer = player0;

    // gui
    float buttonSize = 40;
    Rectangle buttonRect0 = {(float)BOARDER_PADDING, (float)boardStartY, buttonSize, buttonSize};
    Rectangle buttonRect1 = {(float)BOARDER_PADDING, (float)boardStartY + BOARDER_PADDING + buttonSize, buttonSize, buttonSize};

    SetTargetFPS(60);

    bool gameWon = false;

    std::future<Coordinate> solverFuture;
    auto solver = SOLVER_CLASS();

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(GRAY);

        // Game board background
        DrawRectangle(boardStartX, boardStartY, gameBoardWidth, gameBoardHeight, RAYWHITE);

        if (gameWon) {
            auto [startPosition, endPosition] = board->getWinPositions();
            DrawLineEx(startPosition, endPosition, 3.0, RED);
        }

        if (USE_SOLVER && activePlayer == player1 && !solverFuture.valid() && !gameWon) {
            // Start solver.
            solverFuture = std::async(std::launch::async, &SOLVER_CLASS::runSolve, &solver, board, WIN_TARGET);
        }

        if (solverFuture.valid() && solverFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
            // Solver complete.
            Coordinate move = solverFuture.get();

            // Play in the square!
            board->playInCell(activePlayer, move.x, move.y);

            // and change the active player to the next player.
            // TODO, figure out multiple players.
            activePlayer = activePlayer == player0 ? player1 : player0;

            gameWon = board->checkForWin(WIN_TARGET);
        }

        // Render the cells inside the board.
        Vector2 mousePos = GetMousePosition();
        if (!gameWon) {
            auto cells = board->getCells();
            for (auto it = cells.begin(); it != cells.end(); it++) {
                int i = std::distance(cells.begin(), it);
                auto column = *it;
                for (auto itt = column.begin(); itt != column.end(); itt++) {
                    int j = std::distance(column.begin(), itt);
                    Cell cell = *itt;

                    if (!cell.isPlayable()) {
                        continue;
                    }
                    Rectangle rect = cell.getRect();
                    if (CheckCollisionPointRec(mousePos, rect)) {
                        // Highlight the hovered, playable square.
                        DrawRectangleRec(rect, LIGHTGRAY);
                        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                            // Play in the square!
                            board->playInCell(activePlayer, i, j);

                            // and change the active player to the next player.
                            // TODO, figure out multiple players.
                            activePlayer = activePlayer == player0 ? player1 : player0;

                            gameWon = board->checkForWin(WIN_TARGET);
                        }
                    }
                }
            }
        }

        // Draw the grid, and any players that have played within the grid.
        board->drawGrid();
        board->drawPlayers();

        // GUI ------
        DrawRectangleRec(buttonRect0, LIGHTGRAY);
        float iconPosX = buttonRect0.x + buttonRect0.width / 2;
        float iconPosY = buttonRect0.y + buttonRect0.height / 2;
        player0->drawPlayer(buttonRect0.height * 0.6,  {iconPosX, iconPosY});

        if (CheckCollisionPointRec(GetMousePosition(), buttonRect0)) {
            if  (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                activePlayer = player0;
            }
        }

        DrawRectangleRec(buttonRect1, LIGHTGRAY);
        iconPosX = buttonRect1.x + buttonRect1.width / 2;
        iconPosY = buttonRect1.y + buttonRect1.height / 2;
        player1->drawPlayer(buttonRect1.height * 0.6,  {iconPosX, iconPosY});

        if (CheckCollisionPointRec(GetMousePosition(), buttonRect1)) {
            if  (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                activePlayer = player1;
            }
        }

        Rectangle activePlayerRect;
        if (activePlayer == player0) {
            activePlayerRect = buttonRect0;
        } else {
            activePlayerRect = buttonRect1;
        }
        Rectangle highlightRect = {activePlayerRect.x - 1, activePlayerRect.y - 1, activePlayerRect.width + 2, activePlayerRect.height + 2};
        DrawRectangleLinesEx(highlightRect, 2, BEIGE);

        EndDrawing();

    }

    CloseWindow();
}


int main(int argc, char** argv) {
    try {
        run();
    } catch (std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
