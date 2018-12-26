#include "ship.h"

Ship::Ship(int length)
    : m_length(length)
{
    m_body.resize(length);
}

e_Status Ship::shot(int x, int y)
{
    for (auto &cell : m_body)
        if (cell.x == x && cell.y == y) {
            cell.status = e_Status::Destroyed;
            if (isLife())
                return e_Status::Hit;
            else
                return e_Status::Destroyed;
        }
    return e_Status::Life;
}

void Ship::addCell(int id, int x, int y)
{
    m_body[id] = {x, y, e_Status::Life};
}

void Ship::reset()
{
    for (auto &el : m_body)
        el = {0, 0, e_Status::Life};
}

bool Ship::isLife() const
{
    for (const auto &cell : m_body)
        if (cell.status == e_Status::Life)
            return true;

    return false;
}

int Ship::length() const
{
    return m_length;
}
