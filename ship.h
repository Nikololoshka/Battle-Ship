#ifndef SHIP_H
#define SHIP_H

#include <QtWidgets>
#include "settings.h"

class Ship
{
public:
    Ship(int length);
    e_Status shot(int x, int y);
    void addCell(int id, int x, int y);
    void reset();
    bool isLife() const;
    int length() const;

private:
    struct CellShip
    {
        int x;
        int y;
        e_Status status;
    };

    int m_length;
    QVector<CellShip> m_body;
};

#endif // SHIP_H
