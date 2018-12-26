#include "player.h"

Player::Player(QString name, QObject *parent, bool showShips)
    : QObject(parent), m_playerName(name)
{
    m_pGameMapPlayer = new GameMap(330, 330, showShips);
    m_pGameMapPlayer->setVisible(false);

    for (auto &el : Settings::instance().shipsLength())
        m_shipsPlayer.append(new Ship(el));

    connect(m_pGameMapPlayer, &GameMap::clicked, this, &Player::mapClicked);
}

bool Player::isDead() const
{
    for (auto &ship : m_shipsPlayer)
        if (ship->isLife())
            return false;

    return true;
}

void Player::resetMap()
{
    m_pGameMapPlayer->resetStatusMesh();

    for (int i = 0; i < MAP_SIZE; ++i)
        m_shipsPlayer[i]->reset();
}

QString &Player::name()
{
    return m_playerName;
}

QVector<QVector<Cell *> > &Player::gameMap()
{
    return m_pGameMapPlayer->mesh();
}

GameMap *Player::graphicGameMap()
{
    return m_pGameMapPlayer;
}


QVector<Ship *> &Player::ships()
{
    return  m_shipsPlayer;
}
