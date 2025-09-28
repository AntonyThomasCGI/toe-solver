
#define _USE_MATH_DEFINES

#include <math.h>
#include <stdexcept>

#include "player.hpp"


Player::Player(PlayerType playerType, Color color)
    : playerType(playerType), color(color)
{

}


Player::~Player()
{

}


void Player::drawPlayer(float size, Vector2 postion)
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
