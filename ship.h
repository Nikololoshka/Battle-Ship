#ifndef SHIP_H
#define SHIP_H

#include <QtWidgets>

class Ship
{
public:
    Ship(int mainX, int mainY, bool isHor, int length);
    void shot(int x, int y);
    bool isLife() const;

private:
    struct CellShip
    {
        int x;
        int y;
        bool isLife;
    };

    int mainX_;
    int mainY_;
    bool isHor_;
    int length_;
    QVector<CellShip> body_;
};

#endif // SHIP_H
