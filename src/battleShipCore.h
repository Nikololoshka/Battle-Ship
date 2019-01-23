#ifndef BATTLESHIPCORE_H
#define BATTLESHIPCORE_H

#include <QtWidgets>
#include "utilities.h"
#include "settings.h"
#include "player.h"
#include "bot.h"
#include "gameMap.h"
#include "gameMapEditor.h"
#include "turnIndicator.h"

class BattleShipCore : public QObject
{
    Q_OBJECT
public:
    BattleShipCore(QObject *parent = nullptr);
    void setShipsFromEditorToPlayer();
    QVector<QSharedPointer<Ship>> standartShips();
    bool isChange() const;
    void setTurnInterval(int msec);
    void resetGame();

    QSharedPointer<GameMap> playerHumanMap() const;
    QSharedPointer<GameMap> playerBotMap() const;
    QSharedPointer<GameMapEditor> gameMapEditor();
    TurnIndicator *turnIndicator() const;

    void setRandShip(QSharedPointer<GameMap> &map, QVector<QSharedPointer<Ship>> &ships);
    bool setShip(QSharedPointer<GameMap> &map, QSharedPointer<Ship> &ship, int x, int y, bool isHor);

signals:
    void endGame(QString winner);

public slots:
    void generateBotMap();
    void changeBotDifficulty(Bot::e_Difficulty difficulty);
    void turnHuman(int x, int y);

private:
    bool winnerChecker();

private:
    bool m_change;
    bool m_turn;
    QEventLoop m_loop;
    QTimer m_timer;

    Player *m_pPlayerHuman;
    Bot *m_pPlayerBot;

    TurnIndicator *m_pTurnIndicator;
    QSharedPointer<GameMapEditor> m_pGameMapEditor;
};

#endif // BATTLESHIPCORE_H
