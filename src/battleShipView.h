#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <QtWidgets>
#include "appNamespace.h"
#include "settings.h"
#include "battleShipCore.h"
#include "turnIndicator.h"
#include "menuButton.h"
#include "menuSelectedButton.h"
#include "menuDisableButton.h"
#include "gameMap.h"
#include "gameMapDragAndDrop.h"

class BattleShipView : public QGraphicsView
{
    Q_OBJECT
public:
    BattleShipView(QWidget *parent = nullptr);
    BattleShipCore *core() const;

private slots:
    void drawMainMenu(QGraphicsItem *clickedItem);
    void drawSinglePlayerMenu(QGraphicsItem *clickedItem);
    void drawRandomPlaceMenu(QGraphicsItem *clickedItem);
    void drawManualPlaceMenu(QGraphicsItem  *clickedItem);
    void drawGame(QGraphicsItem *clickedItem);

    void exitFromGameToMenu(QGraphicsItem *clickedItem);
    void exitFromPlaceRandomlyMenu(QGraphicsItem *clickedItem);

    void playerWins(QString winnerName);

private:
    QGraphicsRectItem *initMainMenu();
    QGraphicsRectItem *initSinglePlayerMenu();
    QGraphicsRectItem *initRandomPlaceMenu();
    QGraphicsRectItem *initManualPlaceMenu();
    QGraphicsRectItem *initGameMenu();

private:
    QGraphicsScene *m_pScene;

    QGraphicsRectItem *m_pMainMenu;
    QGraphicsRectItem *m_pSinglePlayerMenu;
    QGraphicsRectItem *m_pPlaceRandomlyMenu;
    QGraphicsRectItem *m_pManualPlaceMenu;
    QGraphicsRectItem *m_pGameMenu;

    BattleShipCore *m_pBattleShipCore;
};

#endif // BATTLESHIP_H
