
#define _USE_MATH_DEFINES

#include <iostream>
#include <memory>
#include <math.h>

#include "enums.hpp"

#include "raylib.h"




const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int EXTERNAL_GRID_WIDTH = 3;
const int EXTERNAL_GRID_HEIGHT = 3;

const int PLAYER_COUNT = 2;

const int BOARDER_PADDING = 20;



void drawGrid(Vector4 bounds, int x, int y) {
    float xCellSize = (bounds.z - bounds.x) / x;
    float yCellSize = (bounds.w - bounds.y) / y;

    for (int i = 1; i < x; i++) {
        float posX = i * xCellSize + bounds.x;

        DrawLineEx({posX, bounds.y}, {posX, bounds.w}, 3.0, BLACK);
    }

    int numLinesY = ((y + 1) / 2);
    for (int i = 1; i < y; i++) {
        float posY = i * xCellSize + bounds.y;

        DrawLineEx({bounds.x, posY}, {bounds.z, posY}, 3.0, BLACK);
    }
}


void drawPlayer(PlayerType playerType, float size, Vector2 postion, Color color)
{

    float radius = size / 2.0;

    switch (playerType)
    {
    case NAUGHT:
        {
            DrawRing(postion, radius * 0.7, radius, 0.0, 360.0, 0, color);
            break;
        }
    case CROSS:
        {
            DrawLineEx({postion.x + radius, postion.y + radius}, {postion.x - radius, postion.y - radius}, size * 0.15, color);
            DrawLineEx({postion.x - radius, postion.y + radius}, {postion.x + radius, postion.y - radius}, size * 0.15, color);
            break;
        }
    case SQUARE:
        {
            float startX = postion.x - radius;
            float startY = postion.y - radius;
            DrawRectangle(startX, startY, size, size, color);
            break;
        }
    case CIRCLE:
        {
            DrawCircle(postion.x, postion.y, radius, color);
            break;
        }
    case TRIANGLE:
        {
            float rotation = 30.0 * (M_PI / 180.0);
            Vector2 v1 = {
                static_cast<float>(postion.x + radius * cos(rotation)),
                static_cast<float>(postion.y + radius * sin(rotation))
            };
            Vector2 v2 = {
                static_cast<float>(postion.x + radius * cos(rotation + 2 *  M_PI / 3)),
                static_cast<float>(postion.y + radius * sin(rotation + 2 *  M_PI / 3))
            };
            Vector2 v3 = {
                static_cast<float>(postion.x + radius * cos(rotation + 4 *  M_PI / 3)),
                static_cast<float>(postion.y + radius * sin(rotation + 4 *  M_PI / 3))
            };
            DrawTriangle(v3, v2, v1, color);
            break;
        }
    default:
        throw std::runtime_error("draw for playerType not implemented");
    }
}


void run() {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Toe Solver");

    int gameBoardWidth = SCREEN_WIDTH - BOARDER_PADDING * 2;
    int gameBoardHeight = (gameBoardWidth / EXTERNAL_GRID_WIDTH) * EXTERNAL_GRID_HEIGHT;
    if ((gameBoardHeight + BOARDER_PADDING * 2) > SCREEN_HEIGHT) {
        gameBoardHeight = SCREEN_HEIGHT - BOARDER_PADDING * 2;
        gameBoardWidth = (gameBoardHeight / EXTERNAL_GRID_HEIGHT) * EXTERNAL_GRID_WIDTH;
    }

    int boardStartX = (SCREEN_WIDTH - gameBoardWidth) / 2;
    int boardStartY = BOARDER_PADDING;
float cellSize = gameBoardWidth / EXTERNAL_GRID_WIDTH;
    float playerSize = cellSize * 0.6;

    // Build grid of vectors pointing to the center of the grid position.
    Vector2 playablePositions[EXTERNAL_GRID_WIDTH][EXTERNAL_GRID_HEIGHT];
    for (int i = 0; i < EXTERNAL_GRID_WIDTH; i++) {
        float xCoord = ((i + 1) * cellSize) - (cellSize / 2.0) + boardStartX;
        for (int j = 0; j < EXTERNAL_GRID_HEIGHT; j++) {
            float yCoord = ((j + 1) * cellSize) - (cellSize / 2.0) + boardStartY;
            playablePositions[i][j] = {xCoord, yCoord};
        }
    }

    SetTargetFPS(60);

    float buttonSize = 40;
    Rectangle buttonRect0 = {(float)BOARDER_PADDING, (float)boardStartY, buttonSize, buttonSize};
    Rectangle buttonRect1 = {(float)BOARDER_PADDING, (float)boardStartY + BOARDER_PADDING + buttonSize, buttonSize, buttonSize};

    PlayerType activePlayer = CROSS;

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(GRAY);

        // Game board background
        DrawRectangle(boardStartX, boardStartY, gameBoardWidth, gameBoardHeight, RAYWHITE);

        // Game board grid
        float boundsZ = boardStartX + gameBoardWidth;
        float boundsW = boardStartY + gameBoardHeight; 
        Vector4 bounds = {(float)boardStartX, (float)boardStartY, boundsZ, boundsW};
        drawGrid(bounds, EXTERNAL_GRID_WIDTH, EXTERNAL_GRID_HEIGHT);

        //drawPlayer(SQUARE, playerSize, playablePositions[2][2], MAROON);
        drawPlayer(CROSS, playerSize, playablePositions[2][0], MAROON);
        drawPlayer(NAUGHT, playerSize, playablePositions[0][0], DARKGREEN);
        //drawPlayer(CIRCLE, playerSize, playablePositions[0][0], DARKBLUE);
        //drawPlayer(TRIANGLE, playerSize, playablePositions[0][1], BLACK);

        // GUI ------
        DrawRectangleRec(buttonRect0, LIGHTGRAY);
        float iconPosX = buttonRect0.x + buttonRect0.width / 2;
        float iconPosY = buttonRect0.y + buttonRect0.height / 2;
        drawPlayer(CROSS, buttonRect0.height * 0.6,  {iconPosX, iconPosY}, MAROON);

        if (CheckCollisionPointRec(GetMousePosition(), buttonRect0)) {
            if  (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                activePlayer = CROSS;
            }
        }

        DrawRectangleRec(buttonRect1, LIGHTGRAY);
        iconPosX = buttonRect1.x + buttonRect1.width / 2;
        iconPosY = buttonRect1.y + buttonRect1.height / 2;
        drawPlayer(NAUGHT, buttonRect1.height * 0.6,  {iconPosX, iconPosY}, DARKGREEN);

        if (CheckCollisionPointRec(GetMousePosition(), buttonRect1)) {
            if  (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                activePlayer = NAUGHT;
            }
        }

        if (activePlayer == CROSS) {
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
