#include "ship.h"

Ship::Ship(int mainX, int mainY, bool isHor, int length)
    : mainX_(mainX), mainY_(mainY), isHor_(isHor), length_(length)
{
    if (isHor)
        for (int i = 0; i < length_; ++i)
            body_.append({mainX + i, mainY, true});
    else
        for (int i = 0; i < length_; ++i)
            body_.append({mainX, mainY + i, true});
}

void Ship::shot(int x, int y)
{
    for (auto &cell : body_)
        if (cell.x == x && cell.y == y) {
            cell.isLife = false;
            return;
        }
}

bool Ship::isLife() const
{
    for (const auto &cell : body_)
        if (cell.isLife == true)
            return true;

    return false;
}
