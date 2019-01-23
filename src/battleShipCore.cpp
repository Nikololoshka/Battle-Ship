#include "battleShipCore.h"

BattleShipCore::BattleShipCore(QObject *parent)
    : QObject (parent),
      m_change(false),
      m_turn(false)
{
    m_pPlayerHuman = new Player(Settings::inst().playerName(), this);
    m_pPlayerBot = new Bot(tr("computer"), this);

    m_pGameMapEditor = QSharedPointer<GameMapEditor>::create(510, 330);
    m_pTurnIndicator = new TurnIndicator(60, 60);

    m_pGameMapEditor->setShips(standartShips());

    QSharedPointer<GameMap> mapHuman = QSharedPointer<GameMap>::create(330, 330, true);
    QSharedPointer<GameMap> mapBot = QSharedPointer<GameMap>::create(330, 330);

    m_pPlayerHuman->setMap(mapHuman);
    m_pPlayerHuman->setShips(standartShips());
    m_pPlayerBot->setMap(mapBot);
    m_pPlayerBot->setShips(standartShips());

    m_timer.setInterval(Settings::inst().animationDelay() + 100);

    connect(mapBot.data(), &GameMap::clicked, this, &BattleShipCore::turnHuman);
    connect(&m_timer, &QTimer::timeout, &m_loop, &QEventLoop::quit);
}

void BattleShipCore::setShipsFromEditorToPlayer()
{
    m_pGameMapEditor->moveShipsOnGameMap(m_pPlayerHuman->gameMap(),
                                         m_pPlayerHuman->ships());
}

QVector<QSharedPointer<Ship> > BattleShipCore::standartShips()
{
    QVector<int> shipsLength = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    QVector<QSharedPointer<Ship>> result(shipsLength.size());

    for (int i = 0; i < shipsLength.size(); ++i)
        result[i] = QSharedPointer<Ship>::create(shipsLength[i]);

    return result;
}

bool BattleShipCore::isChange() const
{
    return m_change;
}

void BattleShipCore::setTurnInterval(int msec)
{
    m_timer.setInterval(msec);
}

void BattleShipCore::resetGame()
{
    m_change = false;    
    m_turn = false;
    m_pPlayerBot->reset();
    m_pPlayerHuman->reset();
    m_pTurnIndicator->reset();
}

QSharedPointer<GameMap> BattleShipCore::playerHumanMap() const
{
    return m_pPlayerHuman->gameMap();
}

QSharedPointer<GameMap> BattleShipCore::playerBotMap() const
{
    return  m_pPlayerBot->gameMap();
}

QSharedPointer<GameMapEditor> BattleShipCore::gameMapEditor()
{
    return m_pGameMapEditor;
}
TurnIndicator *BattleShipCore::turnIndicator() const
{
    return m_pTurnIndicator;
}

void BattleShipCore::setRandShip(QSharedPointer<GameMap> &map, QVector<QSharedPointer<Ship> > &ships)
{
    int x = 0, y = 0, dir = 0;

    for (int i = 0; i < 10; i++) {
        do {
            x = generateRandomNumber(0, g_MAP_SIZE - 1);
            y = generateRandomNumber(0, g_MAP_SIZE - 1);
            dir = generateRandomNumber(0, 1);
        } while (!setShip(map, ships[i], x, y, dir));
    }
}

bool BattleShipCore::setShip(QSharedPointer<GameMap> &map, QSharedPointer<Ship> &ship, int x, int y, bool isHor)
{
    bool correct = true;

    if (isHor ? x + ship->length() >= g_MAP_SIZE : y + ship->length() >= g_MAP_SIZE) {
        return false;
    } else {
        for (int i = 0, j = 0; i < ship->length() && j < ship->length(); isHor ? i++ : j++) {
            if (map->cellStatus(x + i, y + j) != e_Status::Empty) {
                correct = false;
                break;
            }
        }
    }
    if (correct) {
        ship->setOrientation(isHor ? Ship::Horizontal : Ship::Vertical);
        for (int a = 0, b = 0; a < ship->length() && b < ship->length(); isHor ? a++ : b++) {
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    if (i == 0 && j == 0) {
                        map->setCellStatus(x + a, y + b, e_Status::Life);
                        map->setCellShip(x + a, y + b, ship);
                        ship->setCellCoord(a + b, x + a, y + b);
                    } else if (x + i + a < g_MAP_SIZE && x + i + a >= 0 &&
                             y + j + b < g_MAP_SIZE && y + j + b >= 0 &&
                             map->cellStatus(x + i + a, y + j + b) != e_Status::Life) {
                        map->setCellStatus(x + i + a, y + j + b, e_Status::NearbyShip);
                    }
                }
            }
        }
        return true;
    }
    return false;
}

void BattleShipCore::turnHuman(int x, int y)
{
    if (!m_turn) {
        m_turn = true;
        m_change = true;
        switch(m_pPlayerBot->gameMap()->shot(x, y)) {  // shoots player human
        case e_Status::Impossible:
        case e_Status::Hit:
            m_turn = false;
            return;
        case e_Status::Destroyed:
            m_pPlayerBot->gameMap()->setDestroyedArea(x, y);
            winnerChecker();
            m_turn = false;
            return;
        default:
            break;
        }
        // if a human missed
        // then the bot shoots
        m_pTurnIndicator->change(180, Qt::red);

        while (true) {
            switch (m_pPlayerBot->turn(m_pPlayerHuman)) {
            case e_Status::Miss:
                m_timer.start();
                m_loop.exec();
                m_pTurnIndicator->change(180, Qt::green);
                m_turn = false;
                return;
            case e_Status::Hit:
                m_timer.start();
                m_loop.exec();
                break;
            case e_Status::Destroyed:
                m_pPlayerHuman->gameMap()->setDestroyedArea(m_pPlayerBot->botX(),
                                                            m_pPlayerBot->botY());
                if (winnerChecker())
                    return;
                m_timer.start();
                m_loop.exec();
                break;
            default:
                break;
            }
        }
    }
}

void BattleShipCore::generateBotMap()
{
    m_pPlayerBot->reset();
    setRandShip(m_pPlayerBot->gameMap(), m_pPlayerBot->ships());
}

void BattleShipCore::changeBotDifficulty(Bot::e_Difficulty difficulty)
{
    m_pPlayerBot->setDifficulty(difficulty);
}

bool BattleShipCore::winnerChecker()
{
    if (m_pPlayerHuman->isDead()) {
        m_change = false;
        emit endGame(m_pPlayerBot->name());
        return true;
    }
    if (m_pPlayerBot->isDead()) {
        m_change = false;
        emit endGame(m_pPlayerHuman->name());
        return true;
    }
    return false;
}



