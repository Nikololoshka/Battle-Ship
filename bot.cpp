#include "bot.h"

Bot::Bot(QString name, QObject *parent)
    : Player (name, parent),
      m_expertMode(false),
      m_changeShotDirection(false),
      m_botX(0), m_botY(0),
      m_primaryX(0), m_primaryY(0),
      m_shotDirection(e_Direction::None)
{
}

e_Status Bot::turn(Player *otherPlayer)
{
    if (!m_expertMode) {
        bool selectCoords = false;
        do {
            // генерация координат для стрельбы
            m_botX = qrand() % MAP_SIZE;
            m_botY = qrand() % MAP_SIZE;

            if (otherPlayer->gameMap()[m_botX][m_botY]->isEmpty())
                selectCoords = true;

        } while (!selectCoords);
    } else {
        do {
            m_changeShotDirection = false;
            // выбор направления стрельбы
            switch (m_shotDirection) {
            case e_Direction::Left:
                if (m_botX > 0) {
                    m_botX--;
                    if (!otherPlayer->gameMap()[m_botX][m_botY]->isEmpty())
                        m_changeShotDirection = true;
                } else
                    m_changeShotDirection = true;
                break;
            case e_Direction::Right:
                if (m_botX < MAP_SIZE - 1) {
                    m_botX++;
                    if (!otherPlayer->gameMap()[m_botX][m_botY]->isEmpty())
                        m_changeShotDirection = true;
                } else
                    m_changeShotDirection = true;
                break;
            case e_Direction::Down:
                if (m_botY > 0) {
                    m_botY--;
                    if (!otherPlayer->gameMap()[m_botX][m_botY]->isEmpty())
                        m_changeShotDirection = true;
                } else
                    m_changeShotDirection = true;
                break;
            case e_Direction::Up:
                if (m_botY < MAP_SIZE - 1) {
                    m_botY++;
                    if (!otherPlayer->gameMap()[m_botX][m_botY]->isEmpty())
                        m_changeShotDirection = true;
                } else
                    m_changeShotDirection = true;
                break;
            default:
                m_expertMode = false;
                break;
            }

            // смена направления стрельбы
            if (m_changeShotDirection) {
                m_shotDirection = static_cast<e_Direction>((static_cast<int>(m_shotDirection) + 1) % 4);
                m_botX = m_primaryX;
                m_botY = m_primaryY;
            }
        } while (m_changeShotDirection);
    }

    auto resultShooting = otherPlayer->gameMap()[m_botX][m_botY]->shot();

    switch (resultShooting) {
    case e_Status::Hit:
        if (!m_expertMode) {
            m_primaryX = m_botX;
            m_primaryY = m_botY;
            m_expertMode = true;
            m_shotDirection = static_cast<e_Direction>(qrand() % 4);
        }
        break;
    case e_Status::Miss:
        m_botX = m_primaryX;
        m_botY = m_primaryY;
        switch (m_shotDirection) {
        case e_Direction::Left:
            m_shotDirection = e_Direction::Right;
            break;
        case e_Direction::Right:
            m_shotDirection = e_Direction::Left;
            break;
        case e_Direction::Down:
            m_shotDirection = e_Direction::Up;
            break;
        case e_Direction::Up:
            m_shotDirection = e_Direction::Down;
            break;
        default:
            break;
        }
        break;
    case e_Status::Destroyed:
        m_expertMode = false;
        break;
    default:
        break;
    }
    return resultShooting;
}


int Bot::getBotX() const
{
    return m_botX;
}

int Bot::getBotY() const
{
    return m_botY;
}
