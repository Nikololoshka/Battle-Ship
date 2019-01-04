#ifndef PLAYER_H
#define PLAYER_H

#include <QtWidgets>
#include "gameMap.h"
#include "ship.h"

class Player : public QObject
{
    Q_OBJECT
public:
    Player(QString name, QObject *parent = nullptr);

    void setMap(GameMap *map);
    void setShips(QVector<Ship *> ships);

    bool isDead() const;
    void reset();
    QString name() const;
    GameMap *gameMap();
    QVector<Ship *> &ships();

signals:
    void mapClicked(int x, int y);

protected:
    QString m_playerName;
    QVector<Ship *> m_shipsPlayer;
    GameMap *m_pGameMapPlayer;
};

#endif // PLAYER_H
