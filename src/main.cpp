

#include <iostream>
#include <memory>

#include "board.hpp"
#include "player.hpp"

#include "raylib.h"


// Window size
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;


// Game rules
const unsigned int EXTERNAL_GRID_WIDTH = 5;
const unsigned int EXTERNAL_GRID_HEIGHT = 5;

const unsigned int PLAYER_COUNT = 2;

const unsigned int WIN_TARGET = 4;


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
    std::unique_ptr<Board> board = std::make_unique<Board>(
        EXTERNAL_GRID_WIDTH, EXTERNAL_GRID_HEIGHT, boardBounds
    );

    std::shared_ptr<Player> player0 = std::make_shared<Player>(CROSS, MAROON);
    std::shared_ptr<Player> player1 = std::make_shared<Player>(NAUGHT, DARKGREEN);

    auto activePlayer = player0;

    // gui
    float buttonSize = 40;
    Rectangle buttonRect0 = {(float)BOARDER_PADDING, (float)boardStartY, buttonSize, buttonSize};
    Rectangle buttonRect1 = {(float)BOARDER_PADDING, (float)boardStartY + BOARDER_PADDING + buttonSize, buttonSize, buttonSize};

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(GRAY);

        // Game board background
        DrawRectangle(boardStartX, boardStartY, gameBoardWidth, gameBoardHeight, RAYWHITE);

        bool won = board->checkForWin(WIN_TARGET);
        if (won) {
            auto [startPosition, endPosition] = board->getWinPositions();
            DrawLineEx(startPosition, endPosition, 3.0, RED);
        }

        // Render the cells inside the board.
        Vector2 mousePos = GetMousePosition();
        if (!won) {
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

        if (activePlayer == player0) {
            DrawRectangleLinesEx(buttonRect0, 1.5, WHITE);
        } else {
            DrawRectangleLinesEx(buttonRect1, 1.5, WHITE);
        }

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
