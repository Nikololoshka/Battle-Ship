#ifndef SHIP_H
#define SHIP_H

#include <QtWidgets>
#include "utilities.h"

class Ship
{
public:
    struct CellShip
    {
        int x;
        int y;
        e_Status status;
    };

    Ship(int length);
    e_Status shot(int x, int y);
    void setCellCoord(int numberCell, int x, int y);
    int numberCell(int x, int y);
    QVector<CellShip> &body();
    void reset();
    bool isLife() const;
    int length() const;

private:
    int m_length;
    QVector<CellShip> m_body;
};

#endif // SHIP_H
