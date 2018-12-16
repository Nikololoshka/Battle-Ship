#include "battleship.h"

BattleShip::BattleShip(QWidget *parent)
    : QMainWindow (parent)
{
    // initialization
    Settings::instance().load();

    view = new QGraphicsView;
    scene = new QGraphicsScene(Settings::instance().sceneSize());
    change = false;
    buttonBack = new MenuButton(tr("Back"), 100, 50);

    // setting
    setMinimumSize(800, 600);

    buttonBack->setPos(0, scene->height() - 50);
    scene->setBackgroundBrush(QBrush(QImage(QStringLiteral(":/bg-image.png"))));

    view->setScene(scene);
    view->fitInView(0, 0, 800, 600);
    view->setRenderHint(QPainter::Antialiasing);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    MenuButton *buttonSingleGame = new MenuButton(tr("Start"), 350, 100);
    MenuButton *buttonLoadGame = new MenuButton(tr("Load game"), 350, 100, MenuButton::ButtonType::Disable);
    MenuButton *buttonMultiplayer = new MenuButton(tr("Multiplayer"), 350, 100, MenuButton::ButtonType::Disable);
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

    drawMesh10x10(mapPlayer1, scene->width() / 2 - 400, scene->height() / 2 - 150, 300, 300, true);
    drawMesh10x10(mapPlayer2, scene->width() / 2 + 100, scene->height() / 2 - 150, 300, 300);
}

void BattleShip::slotCellClicked(int id)
{
    mapPlayer1[id]->shot();
    change = true;
}

void BattleShip::drawMesh10x10(QVector<Cell *> &map, double x, double y, int w, int h, bool disable)
{
    const int size = 10;
    const auto dx = w / size;
    const auto dy = h / size;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            auto temp = new Cell(x + dx * j, y + dy * i, dx, dy, i * size + j,
                                 disable ? Cell::Status::Disable : Cell::Status::Life);
            scene->addItem(temp);
            connect(temp, &Cell::clicked, this, &BattleShip::slotCellClicked);
            map.append(temp);
        }
    }
}

void BattleShip::resetGame()
{
    mapPlayer1.clear();
    mapPlayer2.clear();
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
