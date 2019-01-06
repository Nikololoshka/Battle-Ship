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
    virtual ~Player();

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
    GameMap *m_pGameMapPlayer;
    QVector<Ship *> m_shipsPlayer;
};

#endif // PLAYER_H
