#ifndef SHIP_H
#define SHIP_H

#include <QtWidgets>
#include "battleShipNamespace.h"

class Ship
{
public:
    Ship(int length);
    App::Status shot(int x, int y);
    void addCell(int id, int x, int y);
    void reset();
    bool isLife() const;
    int length() const;

private:
    struct CellShip
    {
        int x;
        int y;
        App::Status status;
    };

    int length_;
    QVector<CellShip> body_;
};

#endif // SHIP_H
