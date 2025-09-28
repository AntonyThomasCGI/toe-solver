#pragma once

#include "raylib.h"

#include "enums.hpp"


class Player
{
public:
    Player(PlayerType playerType, Color color);
    ~Player();

    void drawPlayer(float size, Vector2 postion);

private:
    PlayerType playerType;
    Color color;
};
