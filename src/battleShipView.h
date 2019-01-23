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
#include "gameMapEditor.h"

class BattleShipView : public QGraphicsView
{
    Q_OBJECT
public:
    BattleShipView(QWidget *parent = nullptr);
    BattleShipCore *core() const;

private slots:
    void drawMainMenu(QGraphicsItem *clickedItem);
    void drawSinglePlayerMenu(QGraphicsItem *clickedItem);
    void drawGame(QGraphicsItem *clickedItem);

    void clickedSetClearMode();
    void clickedSetRandomMode();

    void exitFromGameToMenu(QGraphicsItem *clickedItem);
    void playerWins(QString winnerName);

protected:
    void changeEvent(QEvent *event) override;
    bool event(QEvent *event) override;

private:
    QGraphicsRectItem *initMainMenu();
    QGraphicsRectItem *initSinglePlayerMenu();
    QGraphicsRectItem *initGameMenu();

private:
    QGraphicsScene *m_pScene;

    struct {
        QGraphicsRectItem *m_pMainMenu;
        TextLabel *m_pMainMenuTitle;
        MenuButton *m_pButtonSinglePlayer;
        MenuDisableButton *m_pButtonMultiPlayer;
        MenuDisableButton *m_pButtonStatistics;
        MenuButton *m_pButtonExit;
    } mainMenu;

    struct {
        QGraphicsRectItem *m_pSinglePlayerMenu;
        TextLabel *m_pSinglePlayerTitle;
        MenuButton *m_pButtonStartGame;
        MenuSelectedButton *m_pButtonSelectDifficulty;
        MenuButton *m_pButtonClearMap;
        MenuButton *m_pButtonGenerateNewMap;
        MenuButton *m_pButtonSinglePlayerBack;
        QSharedPointer<GameMapEditor> m_pGameMapEditor;
    } singlePlayerMenu;

    struct {
        QGraphicsRectItem *m_pGameMenu;
        MenuButton *m_pButtonGameMenuBack;
        QSharedPointer<GameMap> m_pHumanMap;
        QSharedPointer<GameMap> m_pBotMap;
    } gameMenu;

    BattleShipCore *m_pBattleShipCore;
};

#endif // BATTLESHIP_H
