#ifndef PLAYER_H
#define PLAYER_H

#include <QtWidgets>
#include "graphicElements.h"

class Player : public QObject
{
    Q_OBJECT
public:
    Player(QString name, QObject *parent = nullptr, bool showShips = false);
    bool isDead() const;
    void resetMap();
    QString &name();
    QVector<QVector<Cell *>> &gameMap();
    GameMap *graphicGameMap();
    QVector<Ship *> &ships();

signals:
    void mapClicked(int x, int y);

protected:
    QString m_playerName;
    QVector<Ship *> m_shipsPlayer;
    GameMap *m_pGameMapPlayer;
};

#endif // PLAYER_H
