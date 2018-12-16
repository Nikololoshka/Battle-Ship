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

    void drawMenu();
    void drawGame();

protected:
    void closeEvent(QCloseEvent *event) override;

protected slots:
    void slotCellClicked(int id);

private:
    void drawMesh10x10(QVector<Cell *> &map, double x, double y, int w, int h, bool disable = false);
    void resetGame();

    QGraphicsView *view;
    QGraphicsScene *scene;
    QVector<MenuButton *> menu;
    QVector<Cell *> mapPlayer1;
    QVector<Cell *> mapPlayer2;
    MenuButton *buttonBack;
    bool change;
};

#endif // BATTLESHIP_H
