#include "ship.h"

Ship::Ship(int length)
    : length_(length)
{
    body_.resize(length);
}

App::Status Ship::shot(int x, int y)
{
    for (auto &cell : body_)
        if (cell.x == x && cell.y == y) {
            cell.status = App::Destroyed;
            if (isLife())
                return App::Hit;
            else
                return App::Destroyed;
        }
    return App::Life;
}

void Ship::addCell(int id, int x, int y)
{
    body_[id] = {x, y, App::Life};
}

void Ship::reset()
{
    for (auto &el : body_)
        el = {0, 0, App::Life};
}

bool Ship::isLife() const
{
    for (const auto &cell : body_)
        if (cell.status == App::Life)
            return true;

    return false;
}

int Ship::length() const
{
    return length_;
}
