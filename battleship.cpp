#include "battleship.h"

BattleShip::BattleShip(QWidget *parent)
    : QMainWindow (parent)
{
    // initialization
    Settings::instance().load();

    view = new QGraphicsView;
    scene = new QGraphicsScene(Settings::instance().sceneSize());
    change = false;
    turn = false;
    buttonBack = new MenuButton(tr("Back"), 100, 50);
    indicator = new TurnIndicator(scene->width() / 2 - 30,
                                  scene->height() / 2 - 30,
                                  60, 60);
    for (int i = 0; i < MAP_SIZE; ++i) {
        mapPlayer1.append(QVector<Cell *>(MAP_SIZE, nullptr));
        mapPlayer2.append(QVector<Cell *>(MAP_SIZE, nullptr));
    }

    for (auto &el : Settings::instance().shipsLength()) {
        shipsPlayer1.append(new Ship(el));
        shipsPlayer2.append(new Ship(el));
    }

    // setting
    setMinimumSize(800, 600);

    timer.setInterval(1000);
    buttonBack->setPos(0, scene->height() - 50);
    scene->setBackgroundBrush(QBrush(QImage(QStringLiteral(":/bg-image.png"))));

    view->setScene(scene);
    view->fitInView(0, 0, 800, 600);
    view->setRenderHint(QPainter::Antialiasing);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    MenuButton *buttonSingleGame = new MenuButton(tr("Start"), 350, 100);
    MenuButton *buttonLoadGame = new MenuButton(tr("Load game"), 350, 100, MenuButton::Disable);
    MenuButton *buttonMultiplayer = new MenuButton(tr("Multiplayer"), 350, 100, MenuButton::Disable);
    MenuButton *buttonExit = new MenuButton(tr("Exit"), 350, 100);

    menu.append(buttonSingleGame);
    menu.append(buttonLoadGame);
    menu.append(buttonMultiplayer);
    menu.append(buttonExit);

    double hs = menu.size() * (-50);

    for (int i = 0; i < menu.size(); ++i, hs += 100) {
        menu[i]->setPos(scene->width() / 2 -175, scene->height() / 2 + hs);
        scene->addItem(menu[i]);
    }

    QMenu *menuFile = new QMenu(tr("&File"));
    menuFile->addAction(tr("Exit"), this, &BattleShip::close, QKeySequence("CTRL+Q"));
    menuBar()->addMenu(menuFile);

    QMenu *menuAbout = new QMenu(tr("&About"));
    menuAbout->addAction(tr("About battle ship"), [this](){
        QMessageBox::information(this, tr("About battle ship"),
                                 tr("Create by:\n"
                                    "Nikolay Vereshchagin\n"
                                    "Stankin, IDB-17-09"));
    });
    menuBar()->addMenu(menuAbout);

    // layout setup
    setCentralWidget(view);

    // connections
    connect(buttonSingleGame, &MenuButton::clicked, this, &BattleShip::drawGame);
    connect(buttonExit, &MenuButton::clicked, this, &BattleShip::close);
    connect(buttonBack, &MenuButton::clicked, this, &BattleShip::drawMenu);    
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
}

void BattleShip::drawMenu()
{
    if (change) {
        auto answer = QMessageBox::question(this, tr("Exit"), tr("Are you sure you want to go out?"));

        if (answer == QMessageBox::StandardButton::No)
            return;
    }
    change = false;
    scene->removeItem(buttonBack);
    scene->removeItem(indicator);
    buttonBack->reset();
    scene->clear();
    resetGame();

    for (auto item : menu)
        scene->addItem(item);
}

void BattleShip::drawGame()
{
    for (auto item : menu) {
        scene->removeItem(item);
        item->reset();
    }
    scene->addItem(buttonBack);
    scene->addItem(indicator);

    drawMesh10x10(mapPlayer1, scene->width() / 2 - 430, scene->height() / 2 - 180, 330, 330, true);
    drawMesh10x10(mapPlayer2, scene->width() / 2 + 100, scene->height() / 2 - 180, 330, 330);

    setRandShip(mapPlayer1, shipsPlayer1);
    setRandShip(mapPlayer2, shipsPlayer2);
}

void BattleShip::slotCellClicked(int id)
{
    if (!turn) {    // производится ли ход
        turn = true;
        change = true;
        switch(mapPlayer2[id / 10][id % 10]->shot(id / 10, id % 10)) {  // стреляет игрок 1
        case Ship::Hit:
            turn = false;
            return;
        case Ship::Destroyed:
            drawDestroyedArea(mapPlayer2, id / 10, id % 10);
            turn = false;
            return;
        default:
            break;
        }
        // если промазал
        // то стреляет бот
        indicator->change(TurnIndicator::Left, Qt::red);

        while (true) {
            int botX = rand() % MAP_SIZE;
            int botY = rand() % MAP_SIZE;
            switch (mapPlayer1[botX][botY]->shot(botX, botY)) {
            case Ship::Life:
                timer.start();
                loop.exec();
                indicator->change(TurnIndicator::Right, Qt::green);
                turn = false;
                return;
            case Ship::Hit:
                timer.start();
                loop.exec();
                break;
            case Ship::Destroyed:
                timer.start();
                loop.exec();
                drawDestroyedArea(mapPlayer1, botX, botY);
            }
        }
    }
}

void BattleShip::drawMesh10x10(QVector<QVector<Cell *>> &map, double x, double y, int w, int h, bool disable)
{
    const auto dx = w / (MAP_SIZE + 1);
    const auto dy = h / (MAP_SIZE + 1);

    for (int i = 0; i < MAP_SIZE + 1; ++i) {
        for (int j = 0; j < MAP_SIZE + 1; ++j) {
            if (i == 0 && j == 0) {
                continue;
            } else if (i == 0) {
                scene->addItem(new TextCell(x + dx * j, y + dy * i, dx, dy, QChar('A' + j - 1)));
            } else if (j == 0) {
                scene->addItem(new TextCell(x + dx * j, y + dy * i, dx, dy, QString::number(i)));
            } else {
                auto temp = new Cell(x + dx * j, y + dy * i, dx, dy, (i - 1) * MAP_SIZE + (j - 1), disable);
                scene->addItem(temp);
                connect(temp, &Cell::clicked, this, &BattleShip::slotCellClicked);
                map[i - 1][j - 1] = temp;
            }
        }
    }
}

void BattleShip::drawDestroyedArea(QVector<QVector<Cell *>> &map, int x, int y)
{
    Direction dir = None;
    int tempX = x, tempY = y;
    bool find = false;

    while (true) {
        drawDestroyedAreaImpl(map, x, y, dir);
        find = false;
        while (!find) {
            x = tempX;
            y = tempY;
            ++dir;
            switch (dir) {
            case Right:
                x++;
                break;
            case Left:
                x--;
                break;
            case Up:
                y--;
                break;
            case Down:
                y++;
                break;
            default:
                return;
            }
            qDebug() << x << "\t" << y;
            if (map[x][y]->status() == Cell::Destroyed)
                find = true;
        }
    }
}

void BattleShip::drawDestroyedAreaImpl(QVector<QVector<Cell *> > &map, int x, int y, Direction dir)
{
    bool find;
    do {
        find = false;

        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                if ((i + x) < MAP_SIZE && (i + x) >= 0 &&
                    (j + y) < MAP_SIZE && (j + y >= 0)) {
                    if (map[x + i][y + j]->status() == Cell::NearbyShip)
                        map[x + i][y + j]->setStatus(Cell::Miss);
                    if (map[x + i][y + j]->status() == Cell::Hit)
                        map[x + i][y + j]->setStatus(Cell::Destroyed);
                }
            }
        }
        switch (dir) {
        case Right:
            x++;
            break;
        case Left:
            x--;
            break;
        case Up:
            y--;
            break;
        case Down:
            y++;
            break;
        default:
            return;
        }
        if (map[x][y]->status() == Cell::Destroyed)
            find = true;

    } while (find);
}

void BattleShip::setRandShip(QVector<QVector<Cell *>> &map, QVector<Ship *> &ships)
{
    srand(static_cast<unsigned>(time(nullptr)));

    int x = 0, y = 0, dir = 0;

    for (int i = 0; i < 10; i++) {
        do {
            x = rand() % MAP_SIZE;
            y = rand() % MAP_SIZE;
            dir = 0 + rand() % 2;
        } while (!setShip(map, ships[i], x, y, dir));
    }
}

bool BattleShip::setShip(QVector<QVector<Cell *>> &map, Ship *ship, int x, int y, bool isHor)
{
    bool correct = true;

    if (isHor ? x + ship->length() >= MAP_SIZE : y + ship->length() >= MAP_SIZE) {
        return false;
    } else {
        for (int i = 0, j = 0; i < ship->length() && j < ship->length(); isHor ? i++ : j++) {
            if (map[x + i][y + j]->status() != Cell::Empty) {
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
                        map[x + a][y + b]->setStatus(Cell::Life);
                        map[x + a][y + b]->setShip(ship);
                        ship->addCell(a + b, x + a, y + b);
                    } else if (x + i + a < MAP_SIZE && x + i + a >= 0 &&
                             y + j + b < MAP_SIZE && y + j + b >= 0 &&
                             map[x + i + a][y + j + b]->status() != Cell::Life) {
                        map[x + i + a][y + j + b]->setStatus(Cell::NearbyShip);
                    }
                }
            }
        }
        return true;
    }
    return false;
}

void BattleShip::resetGame()
{
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            mapPlayer1[i][j] = nullptr;
            mapPlayer2[i][j] = nullptr;
        }
    }
    for (int i = 0; i < MAP_SIZE; ++i) {
        shipsPlayer1[i]->reset();
        shipsPlayer2[i]->reset();
    }
}


void BattleShip::closeEvent(QCloseEvent *event)
{
    if (change) {
        auto answer = QMessageBox::question(this, tr("Exit"), tr("Are you sure you want to go out?"));

        if (answer == QMessageBox::StandardButton::No) {
            event->ignore();
            return;
        }
    }
    Settings::instance().save();
    QWidget::closeEvent(event);
}

Direction &operator++(Direction &increm)
{
    return increm = static_cast<Direction>((static_cast<int>(increm) + 1) % 5);
}
