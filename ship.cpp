#include "ship.h"

Ship::Ship(int length)
    : length_(length)
{
    body_.resize(length);
}

Ship::Status Ship::shot(int x, int y)
{
    for (auto &cell : body_)
        if (cell.x == x && cell.y == y) {
            cell.status = Status::Destroyed;
            if (isLife())
                return Status::Hit;
            else
                return Status::Destroyed;
        }
    return Status::Life;
}

void Ship::addCell(int id, int x, int y)
{
    body_[id] = {x, y, Status::Life};
}

void Ship::reset()
{
    for (auto &el : body_)
        el = {0, 0, Status::Life};
}

bool Ship::isLife() const
{
    for (const auto &cell : body_)
        if (cell.status == Status::Life)
            return true;

    return false;
}

int Ship::length() const
{
    return length_;
}
