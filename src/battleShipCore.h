#ifndef BATTLESHIPCORE_H
#define BATTLESHIPCORE_H

#include <QtWidgets>
#include "utilities.h"
#include "settings.h"
#include "player.h"
#include "bot.h"
#include "gameMap.h"
#include "gameMapDragAndDrop.h"

class BattleShipCore : public QObject
{
    Q_OBJECT
public:
    BattleShipCore(QObject *parent = nullptr);
    void setHumanPlayerManualPlaceMap();
    bool isChange() const;
    void resetGame();

    GameMap *playerHumanMap() const;
    GameMap *playerBotMap() const;
    GameMapDragAndDrop *dragAndDropMap();

    void setRandShip(GameMap *map, QVector<Ship *> &ships);
    bool setShip(GameMap *map, Ship *ship, int x, int y, bool isHor);

signals:
    void changeTurn(double angle, QColor color);
    void endGame(QString winner);

private slots:
    void turnHuman(int x, int y);

public slots:
    void generateHumanMap();
    void generateBotMap();
    void changeBotDifficulty(Bot::e_Difficulty difficulty);

private:
    bool winnerChecker();
    void setDestroyedArea(Player *player, int x, int y);
    void setDestroyedAreaImpl(Player *player, int x, int y, e_Direction dir);

private:
    bool m_change;
    bool m_turn;
    QEventLoop m_loop;
    QTimer m_timer;

    Player *m_pPlayerHuman;
    Bot *m_pPlayerBot;

    GameMapDragAndDrop *m_pDragAndDropMap;
};

#endif // BATTLESHIPCORE_H
