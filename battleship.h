#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <QtWidgets>
#include "settings.h"
#include "grahicelements.h"

enum Direction {
    Up,
    Down,
    Left,
    Right,
    None
};

Direction &operator++(Direction &increm);

class BattleShip : public QMainWindow
{
    Q_OBJECT

public:
    BattleShip(QWidget *parent = nullptr);

    void drawMenu();
    void drawGame();

protected:
    void closeEvent(QCloseEvent *event) override;

protected slots:
    void slotCellClicked(int id);

private:
    void drawMesh10x10(QVector<QVector<Cell *>> &map, double x, double y, int w, int h, bool disable = false);
    void drawDestroyedArea(QVector<QVector<Cell *>> &map, int x, int y);
    void drawDestroyedAreaImpl(QVector<QVector<Cell *>> &map, int x, int y, Direction dir);
    void setRandShip(QVector<QVector<Cell *>> &map, QVector<Ship *> &ships);
    bool setShip(QVector<QVector<Cell *>> &map, Ship *ship,
                 int x, int y, bool isHor);
    void resetGame();

    QGraphicsView *view;
    QGraphicsScene *scene;
    QVector<MenuButton *> menu;
    QVector<QVector<Cell *>> mapPlayer1;
    QVector<QVector<Cell *>> mapPlayer2;
    QVector<Ship *> shipsPlayer1;
    QVector<Ship *> shipsPlayer2;
    MenuButton *buttonBack;
    TurnIndicator *indicator;
    QEventLoop loop;
    QTimer timer;
    bool change;
    bool turn;
};

#endif // BATTLESHIP_H
