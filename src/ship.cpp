#include "ship.h"

Ship::Ship(int length, Orientation orient)
    : m_length(length),
      m_orientation(orient)
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

void Ship::setCellCoord(int numberCell, int x, int y)
{
    m_body[numberCell] = {x, y, e_Status::Life};
}

int Ship::numberCell(int x, int y) const
{
    for (int i = 0; i < m_body.size(); ++i) {
        if (m_body[i].x == x && m_body[i].y == y)
            return i;
    }
    return -1;
}

QVector<Ship::CellShip> &Ship::body()
{
    return m_body;
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

Ship::Orientation Ship::orientation() const
{
    return m_orientation;
}

void Ship::setOrientation(const Orientation &orientation)
{
    m_orientation = orientation;
}
