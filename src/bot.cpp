#include "bot.h"

Bot::Bot(QString name, QObject *parent)
    : Player (name, parent),
      m_expertMode(false),
      m_changeShotDirection(false),
      m_botX(0), m_botY(0),
      m_primaryX(0), m_primaryY(0),
      m_shotDirection(e_Direction::None),
      m_difficulty(e_Difficulty::Easy)
{
}

e_Status Bot::turn(Player *otherPlayer)
{
    switch (m_difficulty) {
    case e_Difficulty::Easy:
        return easyDifficulty(otherPlayer);
    case e_Difficulty::Medium:
        return mediumDifficulty(otherPlayer);
    }
    return e_Status::Impossible;
}

void Bot::setDifficulty(e_Difficulty difficulty)
{
    m_difficulty = difficulty;
}


int Bot::botX() const
{
    return m_botX;
}

int Bot::botY() const
{
    return m_botY;
}

e_Status Bot::easyDifficulty(Player *otherPlayer)
{
    bool selectCoords = false;
    do {
        // generation of coordinates for shooting
        m_botX = generateRandomNumber(0, g_MAP_SIZE - 1);
        m_botY = generateRandomNumber(0, g_MAP_SIZE - 1);

        if (otherPlayer->gameMap()->isEmptyCell(m_botX, m_botY))
            selectCoords = true;

    } while (!selectCoords);

    return otherPlayer->gameMap()->shot(m_botX, m_botY);
}

e_Status Bot::mediumDifficulty(Player *otherPlayer)
{
    if (!m_expertMode) {
        bool selectCoords = false;
        do {
            // generation of coordinates for shooting
            m_botX = generateRandomNumber(0, g_MAP_SIZE - 1);
            m_botY = generateRandomNumber(0, g_MAP_SIZE - 1);

            if (otherPlayer->gameMap()->isEmptyCell(m_botX, m_botY))
                selectCoords = true;

        } while (!selectCoords);
    } else {
        do {
            m_changeShotDirection = false;
            // depending on the direction of the shoot
            switch (m_shotDirection) {
            case e_Direction::Left:
                if (m_botX > 0) {
                    m_botX--;
                    if (!otherPlayer->gameMap()->isEmptyCell(m_botX, m_botY))
                        m_changeShotDirection = true;
                } else
                    m_changeShotDirection = true;
                break;
            case e_Direction::Right:
                if (m_botX < g_MAP_SIZE - 1) {
                    m_botX++;
                    if (!otherPlayer->gameMap()->isEmptyCell(m_botX, m_botY))
                        m_changeShotDirection = true;
                } else
                    m_changeShotDirection = true;
                break;
            case e_Direction::Down:
                if (m_botY > 0) {
                    m_botY--;
                    if (!otherPlayer->gameMap()->isEmptyCell(m_botX, m_botY))
                        m_changeShotDirection = true;
                } else
                    m_changeShotDirection = true;
                break;
            case e_Direction::Up:
                if (m_botY < g_MAP_SIZE - 1) {
                    m_botY++;
                    if (!otherPlayer->gameMap()->isEmptyCell(m_botX, m_botY))
                        m_changeShotDirection = true;
                } else
                    m_changeShotDirection = true;
                break;
            default:
                m_expertMode = false;
                break;
            }

            // change the direction of fire
            if (m_changeShotDirection) {
                m_shotDirection = static_cast<e_Direction>((static_cast<int>(m_shotDirection) + 1) % 4);
                m_botX = m_primaryX;
                m_botY = m_primaryY;
                qDebug() << "Bot: change direction " << static_cast<int>(m_shotDirection) << ": " << m_botX << ", " << m_botY;
            }
        } while (m_changeShotDirection);
    }

    auto resultShooting = otherPlayer->gameMap()->shot(m_botX, m_botY);

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
        if (m_expertMode) {
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
