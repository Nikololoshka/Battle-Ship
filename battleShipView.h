#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <QtWidgets>
#include "settings.h"
#include "player.h"
#include "ship.h"
#include "bot.h"
#include "graphicElements.h"

class BattleShip : public QGraphicsView
{
    Q_OBJECT
public:
    BattleShip(QWidget *parent = nullptr);
    void winnerChecker();
    bool isChange();

private slots:
    void slotCellClicked(int x, int y);
    void drawMainMenu(QGraphicsItem *clickedItem);
    void drawSinglePlayerMenu(QGraphicsItem *clickedItem);
    void drawRandomPlaceMenu(QGraphicsItem *clickedItem);
    void drawGame(QGraphicsItem *clickedItem);
    void generateMap();
    void exitFromGameToMenu();
    void exitFromPlaceRandomlyMenu(QGraphicsItem *clickedItem);

private:
    QGraphicsRectItem *initMainMenu();
    QGraphicsRectItem *initSinglePlayerMenu();
    QGraphicsRectItem *initRandomPlaceMenu();
    QGraphicsRectItem *initGameMenu();

    void drawDestroyedArea(Player *player, int x, int y);
    void drawDestroyedAreaImpl(Player *player, int x, int y, e_Direction dir);
    void clearMenuFromScene();

    void setRandShip(Player *player);
    bool setShip(Player *player, Ship *ship, int x, int y, bool isHor);

private:
    QGraphicsScene *m_pScene;
    QGraphicsRectItem *m_pMainMenu;
    QGraphicsRectItem *m_pSinglePlayerMenu;
    QGraphicsRectItem *m_pPlaceRandomlyMenu;

    struct
    {
        QGraphicsRectItem *parent;
        TurnIndicator *indicator;
        MenuButton *buttonBack;
    } m_gameMenu;

    Player *m_pPlayer1;
    Bot *m_pPlayerBot;

    bool m_change;
    bool m_turn;
    QEventLoop m_loop;
    QTimer m_timer;
};

#endif // BATTLESHIP_H
