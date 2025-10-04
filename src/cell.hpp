# pragma once

#include <memory>
#include <utility>

#include "raylib.h"

#include "player.hpp"
#include "datatypes.hpp"


class Cell
{
public:
    Cell(Rectangle rect, int x, int y);
    ~Cell();

    bool isPlayable() { return !isPlayedIn; }
    void setPlayed(std::shared_ptr<Player> player_);

    Vector2 getPosition() { return postion; }
    const Rectangle getRect() const { return rect; }

    Coordinate getCoordinate() { return coordinate; }

    const std::shared_ptr<Player> getPlayer() const { return player; }


private:
    Rectangle rect;
    Vector2 postion;
    Coordinate coordinate;

    bool isPlayedIn = false;

    std::shared_ptr<Player> player = nullptr;
};
