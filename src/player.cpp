#include "player.h"

Player::Player(QString name, QObject *parent)
    : QObject(parent),
      m_playerName(name),
      m_pGameMapPlayer(nullptr)
{
}

void Player::setMap(QSharedPointer<GameMap> &map)
{
    m_pGameMapPlayer = map;
}

void Player::setShips(QVector<QSharedPointer<Ship>> ships)
{
    m_shipsPlayer = ships;
}

bool Player::isDead() const
{
    for (auto &ship : m_shipsPlayer)
        if (ship->isLife())
            return false;

    return true;
}

void Player::reset()
{
    m_pGameMapPlayer->resetStatusMesh();

    for (int i = 0; i < g_MAP_SIZE; ++i)
        m_shipsPlayer[i]->reset();
}

QString Player::name() const
{
    return m_playerName;
}

QSharedPointer<GameMap> &Player::gameMap()
{
    return m_pGameMapPlayer;
}

QVector<QSharedPointer<Ship> > &Player::ships()
{
    return  m_shipsPlayer;
}
