#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <QtWidgets>
#include "settings.h"
#include "grahicelements.h"

class BattleShip : public QMainWindow
{
    Q_OBJECT

public:
    BattleShip(QWidget *parent = nullptr);
    ~BattleShip();

    void drawMenu();
    void drawGame();

private:
    QGraphicsView *view;
    QGraphicsScene *scene;
};

#endif // BATTLESHIP_H
