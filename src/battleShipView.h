#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <QtWidgets>
#include "utilities.h"
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

    void drawGameFromMenualPlace(QGraphicsItem *clickedItem);

    void exitFromGameToMenu(QGraphicsItem *clickedItem);
    void exitFromPlaceRandomlyMenu(QGraphicsItem *clickedItem);

    void playerWins(QString winnerName);

protected:
    void changeEvent(QEvent *event) override;

private:
    QGraphicsRectItem *initMainMenu();
    QGraphicsRectItem *initSinglePlayerMenu();
    QGraphicsRectItem *initRandomPlaceMenu();
    QGraphicsRectItem *initManualPlaceMenu();
    QGraphicsRectItem *initGameMenu();

private:
    QGraphicsScene *m_pScene;

    QGraphicsRectItem *m_pMainMenu;
    TextLabel *m_pMainMenuTitle;
    MenuButton *m_pButtonSinglePlayer;
    MenuDisableButton *m_pButtonMultiPlayer;
    MenuDisableButton *m_pButtonStatistics;
    MenuButton *m_pButtonExit;

    QGraphicsRectItem *m_pSinglePlayerMenu;
    TextLabel *m_pSinglePlayerTitle;
    MenuSelectedButton *m_pButtonSelectDifficulty;
    MenuButton *m_pButtonRandomPlaceMenu;
    MenuButton *m_pButtonManualPlaceMenu;
    MenuButton *m_pButtonSinglePlayerBack;

    QGraphicsRectItem *m_pRandomPlaceMenu;
    TextLabel *m_pRandomPlaceTitle;
    MenuButton *m_pButtonStartGameRPMenu;
    MenuButton *m_pButtonGenerate;
    MenuButton *m_pButtonRandomPlaceBack;

    QGraphicsRectItem *m_pManualPlaceMenu;
    TextLabel *m_pManualPlaceTitle;
    MenuButton *m_pButtonStartGameMPMenu;
    MenuButton *m_pButtonManualPlaceBack;

    QGraphicsRectItem *m_pGameMenu;
    MenuButton *m_pButtonGameMenuBack;

    BattleShipCore *m_pBattleShipCore;
};

#endif // BATTLESHIP_H
