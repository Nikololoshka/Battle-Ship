#include "battleShipView.h"

BattleShip::BattleShip(QWidget *parent)
    : QGraphicsView(parent)
{
    // initialization
    m_pScene = new QGraphicsScene(Settings::instance().sceneSize());
    m_pPlayer1 = new Player(tr("User"), this, true);
    m_pPlayerBot = new Bot(tr("Computer"), this);
    m_change = false;
    m_turn = false;

    m_pScene->addItem(initMainMenu());
    m_pScene->addItem(initSinglePlayerMenu());
    m_pScene->addItem(initRandomPlaceMenu());
    m_pScene->addItem(initGameMenu());

    m_pScene->addItem(m_pPlayer1->graphicGameMap());
    m_pScene->addItem(m_pPlayerBot->graphicGameMap());

    // settings
    m_timer.setInterval(1000);
    m_pScene->setBackgroundBrush(QImage(QStringLiteral(":/bg-image.png")));

    setScene(m_pScene);
    setMinimumSize(900, 600);
    setRenderHint(QPainter::Antialiasing, true);
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    m_pMainMenu->setVisible(true);

    // connections
    connect(&m_timer, &QTimer::timeout, &m_loop, &QEventLoop::quit);
    connect(m_pPlayerBot, &Player::mapClicked, this, &BattleShip::slotCellClicked);
}

void BattleShip::drawMainMenu(QGraphicsItem *clickedItem)
{
    clickedItem->parentItem()->setVisible(false);
    m_pMainMenu->setVisible(true);
}

void BattleShip::drawSinglePlayerMenu(QGraphicsItem *clickedItem)
{
    clickedItem->parentItem()->setVisible(false);
    m_pSinglePlayerMenu->setVisible(true);
}

void BattleShip::drawRandomPlaceMenu(QGraphicsItem *clickedItem)
{
    clickedItem->parentItem()->setVisible(false);
    m_pPlaceRandomlyMenu->setVisible(true);

    m_pPlayer1->graphicGameMap()->setVisible(true);
    m_pPlayer1->graphicGameMap()->setPos(m_pScene->width() / 2, m_pScene->height() / 2 - 180);
    setRandShip(m_pPlayer1);
}

void BattleShip::drawGame(QGraphicsItem *clickedItem)
{
    clickedItem->parentItem()->setVisible(false);
    m_gameMenu.parent->setVisible(true);

    m_pPlayer1->graphicGameMap()->setVisible(true);
    m_pPlayerBot->graphicGameMap()->setVisible(true);

    m_pPlayer1->graphicGameMap()->setPos(m_pScene->width() / 2 - 430, m_pScene->height() / 2 - 180);
    m_pPlayerBot->graphicGameMap()->setPos(m_pScene->width() / 2 + 100, m_pScene->height() / 2 - 180);

    setRandShip(m_pPlayerBot);
}

void BattleShip::generateMap()
{
    m_pPlayer1->resetMap();
    setRandShip(m_pPlayer1);
}

void BattleShip::exitFromGameToMenu()
{
    m_pPlayer1->resetMap();
    m_pPlayerBot->resetMap();

    m_pPlayer1->graphicGameMap()->setVisible(false);
    m_pPlayerBot->graphicGameMap()->setVisible(false);

    drawMainMenu(m_gameMenu.buttonBack);
}

void BattleShip::exitFromPlaceRandomlyMenu(QGraphicsItem *clickedItem)
{
    m_pPlayer1->resetMap();
    m_pPlayer1->graphicGameMap()->setVisible(false);

    drawSinglePlayerMenu(clickedItem);
}

QGraphicsRectItem *BattleShip::initMainMenu()
{
    auto countElements = 4;
    auto buttonWidth = m_pScene->width() / 4;
    auto buttonHeigth = m_pScene->height() / 7;
    auto width = m_pScene->width() / 2 - buttonWidth / 2;
    auto height = m_pScene->height() / 2 - ((countElements + 2) * buttonHeigth) / 2;

    m_pMainMenu = new QGraphicsRectItem();
    m_pMainMenu->setVisible(false);

    TextLabel *titleLabel = new TextLabel(tr("Battle ship"));
    titleLabel->setSize(m_pScene->width(), buttonHeigth);
    titleLabel->setPos(0, height);
    titleLabel->setParentItem(m_pMainMenu);

    MenuButton *buttonSinglePlayer = new MenuButton(tr("Singleplayer"));
    buttonSinglePlayer->setSize(buttonWidth, buttonHeigth);
    buttonSinglePlayer->setPos(width, height += buttonHeigth);
    buttonSinglePlayer->setParentItem(m_pMainMenu);

    MenuButton *buttonMultiPlayer = new MenuButton(tr("Multiplayer"), e_ButtonType::Disable);
    buttonMultiPlayer->setSize(buttonWidth, buttonHeigth);
    buttonMultiPlayer->setPos(width, height += buttonHeigth);
    buttonMultiPlayer->setParentItem(m_pMainMenu);

    MenuButton *buttonStatistics = new MenuButton(tr("Statistics"), e_ButtonType::Disable);
    buttonStatistics->setSize(buttonWidth, buttonHeigth);
    buttonStatistics->setPos(width, height += buttonHeigth);
    buttonStatistics->setParentItem(m_pMainMenu);

    MenuButton *buttonExit = new MenuButton(tr("Exit"));
    buttonExit->setSize(buttonWidth, buttonHeigth);
    buttonExit->setPos(width, height += buttonHeigth);
    buttonExit->setParentItem(m_pMainMenu);

    connect(buttonSinglePlayer, &MenuButton::clicked, this, &BattleShip::drawSinglePlayerMenu);
    connect(buttonExit, &MenuButton::clicked, this->parentWidget(), &QWidget::close);

    return m_pMainMenu;
}

QGraphicsRectItem *BattleShip::initSinglePlayerMenu()
{
    auto countElements = 4;
    auto buttonWidth = m_pScene->width() / 4;
    auto buttonHeigth = m_pScene->height() / 7;
    auto width = m_pScene->width() / 2 - buttonWidth / 2;
    auto height = m_pScene->height() / 2 - ((countElements + 2) * buttonHeigth) / 2;

    m_pSinglePlayerMenu = new QGraphicsRectItem();
    m_pSinglePlayerMenu->setVisible(false);

    TextLabel *titleLabel = new TextLabel(tr("Singleplayer"));
    titleLabel->setSize(m_pScene->width(), buttonHeigth);
    titleLabel->setPos(0, height);
    titleLabel->setParentItem(m_pSinglePlayerMenu);

    MenuSelectedButton *buttonSelectDifficulty = new MenuSelectedButton();
    buttonSelectDifficulty->addOption(tr("Easy"), e_Difficulty::Easy);
    buttonSelectDifficulty->addOption(tr("Medium"), e_Difficulty::Medium);
    buttonSelectDifficulty->addOption(tr("Hard"), e_Difficulty::Hard);
    buttonSelectDifficulty->setSize(buttonWidth, buttonHeigth);
    buttonSelectDifficulty->setPos(width, height += buttonHeigth);
    buttonSelectDifficulty->setParentItem(m_pSinglePlayerMenu);

    MenuButton *buttonRandomPlaceMenu = new MenuButton(tr("Random"));
    buttonRandomPlaceMenu->setSize(buttonWidth, buttonHeigth);
    buttonRandomPlaceMenu->setPos(width, height += buttonHeigth);
    buttonRandomPlaceMenu->setParentItem(m_pSinglePlayerMenu);

    MenuButton *buttonManualPlaceMenu = new MenuButton(tr("Manual"), e_ButtonType::Disable);
    buttonManualPlaceMenu->setSize(buttonWidth, buttonHeigth);
    buttonManualPlaceMenu->setPos(width, height += buttonHeigth);
    buttonManualPlaceMenu->setParentItem(m_pSinglePlayerMenu);

    MenuButton *buttonSinglePlayerBack = new MenuButton(tr("Back"));
    buttonSinglePlayerBack->setSize(buttonWidth, buttonHeigth);
    buttonSinglePlayerBack->setPos(width, height += buttonHeigth);
    buttonSinglePlayerBack->setParentItem(m_pSinglePlayerMenu);

    connect(buttonSelectDifficulty, &MenuSelectedButton::changeDifficulty, m_pPlayerBot, &Bot::setDifficulty);
    connect(buttonRandomPlaceMenu, &MenuButton::clicked, this, &BattleShip::drawRandomPlaceMenu);
    connect(buttonSinglePlayerBack, &MenuButton::clicked, this, &BattleShip::drawMainMenu);

    return m_pSinglePlayerMenu;
}

QGraphicsRectItem *BattleShip::initRandomPlaceMenu()
{
    auto countElements = 3;
    auto buttonWidth = m_pScene->width() / 4;
    auto buttonHeigth = m_pScene->height() / 7;
    auto width = m_pScene->width() / 4 - buttonWidth / 2;
    auto height = m_pScene->height() / 2 - ((countElements + 2) * buttonHeigth) / 2;

    m_pPlaceRandomlyMenu = new QGraphicsRectItem();
    m_pPlaceRandomlyMenu->setVisible(false);

    TextLabel *titleLabel = new TextLabel(tr("Random placement"));
    titleLabel->setSize(m_pScene->width(), buttonHeigth);
    titleLabel->setPos(0, height - 50);
    titleLabel->setParentItem(m_pPlaceRandomlyMenu);

    MenuButton *buttonStartGame = new MenuButton(tr("Start"));
    buttonStartGame->setSize(buttonWidth, buttonHeigth);
    buttonStartGame->setPos(width, height += buttonHeigth);
    buttonStartGame->setParentItem(m_pPlaceRandomlyMenu);

    MenuButton *buttonGenerate = new MenuButton(tr("Generate"));
    buttonGenerate->setSize(buttonWidth, buttonHeigth);
    buttonGenerate->setPos(width, height += buttonHeigth);
    buttonGenerate->setParentItem(m_pPlaceRandomlyMenu);

    MenuButton *buttonPlaceRandomlyBack = new MenuButton(tr("Back"));
    buttonPlaceRandomlyBack->setSize(buttonWidth, buttonHeigth);
    buttonPlaceRandomlyBack->setPos(width, height += buttonHeigth);
    buttonPlaceRandomlyBack->setParentItem(m_pPlaceRandomlyMenu);

    connect(buttonStartGame, &MenuButton::clicked, this, &BattleShip::drawGame);
    connect(buttonGenerate, &MenuButton::clicked, this, &BattleShip::generateMap);
    connect(buttonPlaceRandomlyBack, &MenuButton::clicked, this, &BattleShip::exitFromPlaceRandomlyMenu);

    return m_pPlaceRandomlyMenu;
}

QGraphicsRectItem *BattleShip::initGameMenu()
{
    auto width = m_pScene->width() / 2;
    auto height = m_pScene->height() / 2;

    m_gameMenu.parent = new QGraphicsRectItem();
    m_gameMenu.parent->setVisible(false);

    m_gameMenu.indicator = new TurnIndicator(width - 30, height - 30, 60, 60);
    m_gameMenu.indicator->setParentItem(m_gameMenu.parent);

    m_gameMenu.buttonBack = new MenuButton(tr("Back"));
    m_gameMenu.buttonBack->setSize(100, 60);
    m_gameMenu.buttonBack->setPos(0, m_pScene->height() - 60);
    m_gameMenu.buttonBack->setParentItem(m_gameMenu.parent);

    connect(m_gameMenu.buttonBack, &MenuButton::clicked, this, &BattleShip::exitFromGameToMenu);

    return m_gameMenu.parent;
}

void BattleShip::winnerChecker()
{
    if (m_pPlayer1->isDead()) {
        QMessageBox::information(this, tr("Win!"), tr("Player ") + m_pPlayerBot->name() + tr(" is winner!"));
        m_change = false;
        exitFromGameToMenu();
    }
    if (m_pPlayerBot->isDead()) {
        QMessageBox::information(this, tr("Win!"), tr("Player ") + m_pPlayer1->name() + tr(" is winner!"));
        m_change = false;
        exitFromGameToMenu();
    }
}

bool BattleShip::isChange()
{
    return m_change;
}

void BattleShip::slotCellClicked(int x, int y)
{
    if (!m_turn) {    // производится ли ход
        m_turn = true;
        m_change = true;
        switch(m_pPlayerBot->gameMap()[x][y]->shot()) {  // стреляет игрок 1
        case e_Status::Impossible:
        case e_Status::Hit:
            m_turn = false;
            return;
        case e_Status::Destroyed:
            drawDestroyedArea(m_pPlayerBot, x, y);
            winnerChecker();
            m_turn = false;
            return;
        default:
            break;
        }
        // если промазал
        // то стреляет бот
        m_gameMenu.indicator->change(e_Direction::Left, Qt::red);

        while (true) {
            switch (m_pPlayerBot->turn(m_pPlayer1)) {
            case e_Status::Miss:
                m_timer.start();
                m_loop.exec();
                m_gameMenu.indicator->change(e_Direction::Right, Qt::green);
                m_turn = false;
                return;
            case e_Status::Hit:
                m_timer.start();
                m_loop.exec();
                break;
            case e_Status::Destroyed:
                drawDestroyedArea(m_pPlayer1,
                                  m_pPlayerBot->getBotX(),
                                  m_pPlayerBot->getBotY());
                winnerChecker();
                m_timer.start();
                m_loop.exec();
                break;
            default:
                break;
            }
        }
    }
}

void BattleShip::drawDestroyedArea(Player *player, int x, int y)
{
    e_Direction dir = e_Direction::None;
    int tempX = x, tempY = y;
    bool find = false;

    while (true) {
        drawDestroyedAreaImpl(player, x, y, dir);
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
            if (x >= MAP_SIZE || y >= MAP_SIZE || x < 0 || y < 0)
                continue;

            if (player->gameMap()[x][y]->status() == e_Status::Destroyed)
                find = true;
        }
    }
}

void BattleShip::drawDestroyedAreaImpl(Player *player, int x, int y, e_Direction dir)
{
    bool find;
    do {
        find = false;

        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                if ((i + x) < MAP_SIZE && (i + x) >= 0 &&
                    (j + y) < MAP_SIZE && (j + y) >= 0) {
                    if (player->gameMap()[x + i][y + j]->status() == e_Status::NearbyShip)
                        player->gameMap()[x + i][y + j]->setStatus(e_Status::Miss);
                    if (player->gameMap()[x + i][y + j]->status() == e_Status::Hit)
                        player->gameMap()[x + i][y + j]->setStatus(e_Status::Destroyed);
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
        if (x >= MAP_SIZE || y >= MAP_SIZE || x < 0 || y < 0)
            continue;

        if (player->gameMap()[x][y]->status() == e_Status::Destroyed)
            find = true;

    } while (find);
}

void BattleShip::setRandShip(Player *player)
{
    int x = 0, y = 0, dir = 0;

    for (int i = 0; i < 10; i++) {
        do {
            x = qrand() % MAP_SIZE;
            y = qrand() % MAP_SIZE;
            dir = qrand() % 2;
        } while (!setShip(player, player->ships()[i], x, y, dir));
    }
}

bool BattleShip::setShip(Player *player, Ship *ship, int x, int y, bool isHor)
{
//    qDebug() << "----------------------";
//    for (int i = 0; i < MAP_SIZE; ++i) {
//        QString temp = "";
//        for (int j = 0; j < MAP_SIZE; ++j) {
//            if (player->gameMap()[i][j]->status() == App::Life)
//                temp.append("#");
//            else if (player->gameMap()[i][j]->status() == App::NearbyShip)
//                temp.append("*");
//            else
//                temp.append("-");
//        }
//        qDebug() << temp;
//    }

    bool correct = true;

    if (isHor ? x + ship->length() >= MAP_SIZE : y + ship->length() >= MAP_SIZE) {
        return false;
    } else {
        for (int i = 0, j = 0; i < ship->length() && j < ship->length(); isHor ? i++ : j++) {
            if (player->gameMap()[x + i][y + j]->status() != e_Status::Empty) {
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
                        player->gameMap()[x + a][y + b]->setStatus(e_Status::Life);
                        player->gameMap()[x + a][y + b]->setShip(ship);
                        ship->addCell(a + b, x + a, y + b);
                    } else if (x + i + a < MAP_SIZE && x + i + a >= 0 &&
                             y + j + b < MAP_SIZE && y + j + b >= 0 &&
                             player->gameMap()[x + i + a][y + j + b]->status() != e_Status::Life) {
                        player->gameMap()[x + i + a][y + j + b]->setStatus(e_Status::NearbyShip);
                    }
                }
            }
        }
        return true;
    }
    return false;
}

