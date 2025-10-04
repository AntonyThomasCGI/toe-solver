#pragma once

#include <string>

#include "raylib.h"

#include "datatypes.hpp"


class Player
{
public:
    Player(std::string id, PlayerType playerType, Color color);
    ~Player();

    void drawPlayer(float size, Vector2 postion);

    std::string getId() { return id; }

private:
    std::string id;
    PlayerType playerType;
    Color color;
};
