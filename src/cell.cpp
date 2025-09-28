
#include <iostream>

#include "cell.hpp"


Cell::Cell(Rectangle rect)
    : rect(rect)
{
    float posX, posY;
    posX = rect.x + rect.width / 2.0;
    posY = rect.y + rect.height / 2.0;

    postion = {posX, posY};
}

Cell::~Cell()
{

}


void Cell::setPlayed(std::shared_ptr<Player> player_)
{

    if (isPlayedIn) {
        throw std::runtime_error("Attempted to play in a played-in cell");
    }

    player = std::move(player_);
    isPlayedIn = true;
}