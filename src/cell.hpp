# pragma once

#include <memory>

#include "raylib.h"

#include "player.hpp"


class Cell
{
public:
    Cell(Rectangle rect);
    ~Cell();

    bool isPlayable() { return !isPlayedIn; }
    void setPlayed(std::shared_ptr<Player> player_);

    Vector2 getPosition() { return postion; }
    Rectangle getRect() { return rect; }

    const std::shared_ptr<Player> getPlayer() const { return player; }

private:
    Rectangle rect;
    Vector2 postion;

    bool isPlayedIn = false;

    std::shared_ptr<Player> player = nullptr;
};
