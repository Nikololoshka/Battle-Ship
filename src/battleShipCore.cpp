#include "battleShipCore.h"

BattleShipCore::BattleShipCore(QObject *parent)
    : QObject (parent),
      m_change(false),
      m_turn(false)
{
    m_pPlayerHuman = new Player(tr("User"), this);
    m_pPlayerBot = new Bot(tr("Computer"), this);

    GameMap *mapHuman = new GameMap(330, 330, true);
    GameMap *mapBot = new GameMap(330, 330);

    m_pPlayerHuman->setMap(mapHuman);
    m_pPlayerHuman->setShips(Settings::inst().standartShips());
    m_pPlayerBot->setMap(mapBot);
    m_pPlayerBot->setShips(Settings::inst().standartShips());

    m_timer.setInterval(Settings::inst().animationDelay() + 100);

    connect(mapBot, &GameMap::clicked, this, &BattleShipCore::turnHuman);
    connect(&m_timer, &QTimer::timeout, &m_loop, &QEventLoop::quit);
}

bool BattleShipCore::isChange() const
{
    return m_change;
}

void BattleShipCore::resetGame()
{
    m_change = false;
    m_pPlayerBot->reset();
    m_pPlayerHuman->reset();
}

GameMap *BattleShipCore::playerHumanMap() const
{
    return m_pPlayerHuman->gameMap();
}

GameMap *BattleShipCore::playerBotMap() const
{
    return  m_pPlayerBot->gameMap();
}

void BattleShipCore::setRandShip(GameMap *map, QVector<Ship *> &ships)
{
    int x = 0, y = 0, dir = 0;

    for (int i = 0; i < 10; i++) {
        do {
            x = qrand() % g_MAP_SIZE;
            y = qrand() % g_MAP_SIZE;
            dir = qrand() % 2;
        } while (!setShip(map, ships[i], x, y, dir));
    }
}

bool BattleShipCore::setShip(GameMap *map, Ship *ship, int x, int y, bool isHor)
{
    //    qDebug() << "----------------------";
    //    for (int i = 0; i < MAP_SIZE; ++i) {
    //        QString temp = "";
    //        for (int j = 0; j < MAP_SIZE; ++j) {
    //            if (player->gameMap()->cellStatus(i, j) == e_Status::Life)
    //                temp.append("#");
    //            else if (player->gameMap()->cellStatus(i, j) == e_Status::NearbyShip)
    //                temp.append("*");
    //            else
    //                temp.append("-");
    //        }
    //        qDebug() << temp;
    //    }

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
    if (!m_turn) {    // производится ли ход
        m_turn = true;
        m_change = true;
        switch(m_pPlayerBot->gameMap()->shot(x, y)) {  // стреляет игрок 1
        case e_Status::Impossible:
        case e_Status::Hit:
            m_turn = false;
            return;
        case e_Status::Destroyed:
            setDestroyedArea(m_pPlayerBot, x, y);
            winnerChecker();
            m_turn = false;
            return;
        default:
            break;
        }
        // если промазал
        // то стреляет бот
        emit changeTurn(e_Direction::Left, Qt::red);

        while (true) {
            switch (m_pPlayerBot->turn(m_pPlayerHuman)) {
            case e_Status::Miss:
                m_timer.start();
                m_loop.exec();
                emit changeTurn(e_Direction::Right, Qt::green);
                m_turn = false;
                return;
            case e_Status::Hit:
                m_timer.start();
                m_loop.exec();
                break;
            case e_Status::Destroyed:
                setDestroyedArea(m_pPlayerHuman,
                                  m_pPlayerBot->getBotX(),
                                  m_pPlayerBot->getBotY());
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

void BattleShipCore::generateHumanMap()
{
    m_pPlayerHuman->reset();
    setRandShip(m_pPlayerHuman->gameMap(), m_pPlayerHuman->ships());
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

void BattleShipCore::setDestroyedArea(Player *player, int x, int y)
{
    e_Direction dir = e_Direction::None;
    int tempX = x, tempY = y;
    bool find = false;

    while (true) {
        setDestroyedAreaImpl(player, x, y, dir);
        find = false;
        while (!find) {
            x = tempX;
            y = tempY;
            dir = static_cast<e_Direction>((static_cast<int>(dir) + 1) % 5);
            switch (dir) {
            case e_Direction::Right:
                x++;
                break;
            case e_Direction::Left:
                x--;
                break;
            case e_Direction::Up:
                y--;
                break;
            case e_Direction::Down:
                y++;
                break;
            default:
                return;
            }
            if (x >= g_MAP_SIZE || y >= g_MAP_SIZE || x < 0 || y < 0)
                continue;

            if (player->gameMap()->cellStatus(x, y) == e_Status::Destroyed)
                find = true;
        }
    }
}

void BattleShipCore::setDestroyedAreaImpl(Player *player, int x, int y, e_Direction dir)
{
    bool find;
    do {
        find = false;

        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                if ((i + x) < g_MAP_SIZE && (i + x) >= 0 &&
                    (j + y) < g_MAP_SIZE && (j + y) >= 0) {
                    if (player->gameMap()->cellStatus(x + i, y + j) == e_Status::NearbyShip)
                        player->gameMap()->setCellStatus(x + i, y + j, e_Status::Miss);
                    if (player->gameMap()->cellStatus(x + i, y + j) == e_Status::Hit)
                        player->gameMap()->setCellStatus(x + i, y + j, e_Status::Destroyed);
                }
            }
        }
        switch (dir) {
        case e_Direction::Right:
            x++;
            break;
        case e_Direction::Left:
            x--;
            break;
        case e_Direction::Up:
            y--;
            break;
        case e_Direction::Down:
            y++;
            break;
        default:
            return;
        }
        if (x >= g_MAP_SIZE || y >= g_MAP_SIZE || x < 0 || y < 0)
            continue;

        if (player->gameMap()->cellStatus(x, y) == e_Status::Destroyed)
            find = true;

    } while (find);
}
