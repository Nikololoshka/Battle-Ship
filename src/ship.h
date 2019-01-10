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
    enum Orientation {
        Vertical,
        Horizontal,
        None
    };

    Ship(int length, Orientation orient = Orientation::None);
    e_Status shot(int x, int y);

    Orientation orientation() const;
    void setOrientation(const Orientation &orientation);
    void setCellCoord(int numberCell, int x, int y);

    int numberCell(int x, int y) const;
    QVector<CellShip> &body();

    void reset();
    bool isLife() const;
    int length() const;

private:
    int m_length;
    Orientation m_orientation;
    QVector<CellShip> m_body;
};

#endif // SHIP_H
