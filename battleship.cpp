#include "battleship.h"

BattleShip::BattleShip(QWidget *parent)
    : QMainWindow (parent)
{
    // initialization
    Settings::instance().load();

    view = new QGraphicsView;
    scene = new QGraphicsScene(Settings::instance().sceneSize());

    // setting

    setMinimumSize(540, 360);
    view->setScene(scene);

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


    drawMenu();
}

BattleShip::~BattleShip()
{
    Settings::instance().save();
}

void BattleShip::drawMenu()
{
    double ws = scene->width() / 2 - 100;
    double hs = scene->width() / 2 - 100;

    MenuButton *buttonGame = new MenuButton(tr("Start"), ws, hs, 200, 100);
    MenuButton *buttonExit = new MenuButton(tr("Exit"), ws, hs + 100, 200, 100);

    buttonGame->setPos(ws, hs);

    scene->addItem(buttonGame);
    scene->addItem(buttonExit);

    connect(buttonGame, &MenuButton::clicked, [](){
        qDebug() << "clicked";
    });
    connect(buttonExit, &MenuButton::clicked, this, &BattleShip::close);
}

void BattleShip::drawGame()
{

}
