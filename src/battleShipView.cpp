#include "battleShipView.h"

BattleShipView::BattleShipView(QWidget *parent)
    : QGraphicsView(parent)
{
    // initialization
    m_pScene = new QGraphicsScene(Settings::inst().sceneRect());
    m_pBattleShipCore = new BattleShipCore(this);
    m_pScene->addItem(m_pBattleShipCore->playerHumanMap());
    m_pScene->addItem(m_pBattleShipCore->playerBotMap());

    m_pScene->addItem(initMainMenu());
    m_pScene->addItem(initSinglePlayerMenu());
    m_pScene->addItem(initRandomPlaceMenu());
    m_pScene->addItem(initManualPlaceMenu());
    m_pScene->addItem(initGameMenu());    

    // settings
    m_pScene->setBackgroundBrush(QImage(QStringLiteral(":/image/bg-image")));

    setScene(m_pScene);
    setMinimumSize(800, 600);
    setRenderHint(QPainter::Antialiasing, true);
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    m_pMainMenu->setVisible(true);

    connect(m_pBattleShipCore, &BattleShipCore::endGame, this, &BattleShipView::playerWins);
}

BattleShipCore *BattleShipView::core() const
{
    return m_pBattleShipCore;
}

void BattleShipView::drawMainMenu(QGraphicsItem *clickedItem)
{
    clickedItem->parentItem()->setVisible(false);
    m_pMainMenu->setVisible(true);
}

void BattleShipView::drawSinglePlayerMenu(QGraphicsItem *clickedItem)
{
    clickedItem->parentItem()->setVisible(false);
    m_pSinglePlayerMenu->setVisible(true);
}

void BattleShipView::drawRandomPlaceMenu(QGraphicsItem *clickedItem)
{
    clickedItem->parentItem()->setVisible(false);
    m_pRandomPlaceMenu->setVisible(true);

    m_pBattleShipCore->playerHumanMap()->setVisible(true);
    m_pBattleShipCore->playerHumanMap()->setPos(m_pScene->width() / 2, m_pScene->height() / 2 - 180);
    m_pBattleShipCore->generateHumanMap();
}

void BattleShipView::drawManualPlaceMenu(QGraphicsItem *clickedItem)
{
    clickedItem->parentItem()->setVisible(false);
    m_pManualPlaceMenu->setVisible(true);
}

void BattleShipView::drawGame(QGraphicsItem *clickedItem)
{
    clickedItem->parentItem()->setVisible(false);
    m_pGameMenu->setVisible(true);

    m_pBattleShipCore->playerHumanMap()->setVisible(true);
    m_pBattleShipCore->playerHumanMap()->setPos(m_pScene->width() / 2 - 430, m_pScene->height() / 2 - 180);
    m_pBattleShipCore->playerBotMap()->setVisible(true);
    m_pBattleShipCore->playerBotMap()->setPos(m_pScene->width() / 2 + 100, m_pScene->height() / 2 - 180);
    m_pBattleShipCore->generateBotMap();
}

void BattleShipView::drawGameFromMenualPlace(QGraphicsItem *clickedItem)
{
    m_pBattleShipCore->setHumanPlayerManualPlaceMap();
    drawGame(clickedItem);
}

void BattleShipView::exitFromGameToMenu(QGraphicsItem *clickedItem)
{
    if (m_pBattleShipCore->isChange()) {
        auto answer = QMessageBox::question(this, tr("Exit to the main menu"),
                                            tr("Are you sure you want to go out?",
                                               "Go to the main menu"));

        if (answer == QMessageBox::StandardButton::No)
            return;
    }
    m_pBattleShipCore->resetGame();
    m_pBattleShipCore->playerHumanMap()->setVisible(false);
    m_pBattleShipCore->playerBotMap()->setVisible(false);
    drawMainMenu(clickedItem);
}

void BattleShipView::exitFromPlaceRandomlyMenu(QGraphicsItem *clickedItem)
{
    clickedItem->parentItem()->setVisible(false);
    m_pBattleShipCore->playerHumanMap()->setVisible(false);
    m_pSinglePlayerMenu->setVisible(true);
}

void BattleShipView::playerWins(QString winnerName)
{
    QMessageBox::information(this, tr("Win!"), tr("Player ") + winnerName + tr(" is winner!"));

    exitFromGameToMenu(m_pGameMenu->childItems().first());
}

void BattleShipView::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        // main menu update
        m_pMainMenuTitle->setText(QApplication::applicationDisplayName());
        m_pButtonSinglePlayer->setText(tr("Singleplayer"));
        m_pButtonMultiPlayer->setText(tr("Multiplayer"));
        m_pButtonStatistics->setText(tr("Statistics"));
        m_pButtonExit->setText(tr("Exit"));

        // singleplayer menu update
        m_pSinglePlayerTitle->setText(tr("Singleplayer"));
        m_pButtonSelectDifficulty->updateTranslate(tr("Easy"), Bot::Easy);
        m_pButtonSelectDifficulty->updateTranslate(tr("Medium"), Bot::Medium);
        m_pButtonRandomPlaceMenu->setText(tr("Random placement"));
        m_pButtonManualPlaceMenu->setText(tr("Manual placement"));
        m_pButtonSinglePlayerBack->setText(tr("Back"));

        // random place menu update
        m_pRandomPlaceTitle->setText(tr("Random placement"));
        m_pButtonStartGameRPMenu->setText(tr("Start"));
        m_pButtonGenerate->setText(tr("Generate"));
        m_pButtonRandomPlaceBack->setText(tr("Back"));

        // manual place menu update
        m_pManualPlaceTitle->setText(tr("Manual placement"));
        m_pButtonStartGameMPMenu->setText(tr("Start"));
        m_pButtonManualPlaceBack->setText(tr("Back"));

        // game menu update
        m_pButtonGameMenuBack->setText(tr("Back"));
    } else {
        QWidget::changeEvent(event);
    }
}

QGraphicsRectItem *BattleShipView::initMainMenu()
{
    auto countElements = 4;
    auto buttonWidth = m_pScene->width() / 2;
    auto buttonHeigth = m_pScene->height() / 8;
    auto width = m_pScene->width() / 2 - buttonWidth / 2;
    auto height = m_pScene->height() / 2 - ((countElements + 2) * buttonHeigth) / 2;

    m_pMainMenu = new QGraphicsRectItem();
    m_pMainMenu->setVisible(false);

    m_pMainMenuTitle = new TextLabel(QApplication::applicationDisplayName());
    m_pMainMenuTitle->setSize(m_pScene->width(), buttonHeigth);
    m_pMainMenuTitle->setPos(0, height - 50);
    m_pMainMenuTitle->setParentItem(m_pMainMenu);

    m_pButtonSinglePlayer = new MenuButton(tr("Singleplayer"));
    m_pButtonSinglePlayer->setSize(buttonWidth, buttonHeigth);
    m_pButtonSinglePlayer->setPos(width, height += buttonHeigth);
    m_pButtonSinglePlayer->setParentItem(m_pMainMenu);

    m_pButtonMultiPlayer = new MenuDisableButton(tr("Multiplayer"));
    m_pButtonMultiPlayer->setSize(buttonWidth, buttonHeigth);
    m_pButtonMultiPlayer->setPos(width, height += buttonHeigth);
    m_pButtonMultiPlayer->setParentItem(m_pMainMenu);

    m_pButtonStatistics = new MenuDisableButton(tr("Statistics"));
    m_pButtonStatistics->setSize(buttonWidth, buttonHeigth);
    m_pButtonStatistics->setPos(width, height += buttonHeigth);
    m_pButtonStatistics->setParentItem(m_pMainMenu);

    m_pButtonExit = new MenuButton(tr("Exit"));
    m_pButtonExit->setSize(buttonWidth, buttonHeigth);
    m_pButtonExit->setPos(width, height += buttonHeigth);
    m_pButtonExit->setParentItem(m_pMainMenu);

    connect(m_pButtonSinglePlayer, &MenuButton::clicked, this, &BattleShipView::drawSinglePlayerMenu);
    connect(m_pButtonExit, &MenuButton::clicked, this->parentWidget(), &QWidget::close);

    return m_pMainMenu;
}

QGraphicsRectItem *BattleShipView::initSinglePlayerMenu()
{
    auto countElements = 4;
    auto buttonWidth = m_pScene->width() / 2;
    auto buttonHeigth = m_pScene->height() / 8;
    auto width = m_pScene->width() / 2 - buttonWidth / 2;
    auto height = m_pScene->height() / 2 - ((countElements + 2) * buttonHeigth) / 2;

    m_pSinglePlayerMenu = new QGraphicsRectItem();
    m_pSinglePlayerMenu->setVisible(false);

    m_pSinglePlayerTitle = new TextLabel(tr("Singleplayer"));
    m_pSinglePlayerTitle->setSize(m_pScene->width(), buttonHeigth);
    m_pSinglePlayerTitle->setPos(0, height - 50);
    m_pSinglePlayerTitle->setParentItem(m_pSinglePlayerMenu);

    m_pButtonSelectDifficulty = new MenuSelectedButton(buttonWidth, buttonHeigth);
    m_pButtonSelectDifficulty->addOption(tr("Easy"), Bot::Easy);
    m_pButtonSelectDifficulty->addOption(tr("Medium"), Bot::Medium);
    m_pButtonSelectDifficulty->setPos(width, height += buttonHeigth);
    m_pButtonSelectDifficulty->setParentItem(m_pSinglePlayerMenu);

    m_pButtonRandomPlaceMenu = new MenuButton(tr("Random placement"));
    m_pButtonRandomPlaceMenu->setSize(buttonWidth, buttonHeigth);
    m_pButtonRandomPlaceMenu->setPos(width, height += buttonHeigth);
    m_pButtonRandomPlaceMenu->setParentItem(m_pSinglePlayerMenu);

    m_pButtonManualPlaceMenu = new MenuButton(tr("Manual placement"));
    m_pButtonManualPlaceMenu->setSize(buttonWidth, buttonHeigth);
    m_pButtonManualPlaceMenu->setPos(width, height += buttonHeigth);
    m_pButtonManualPlaceMenu->setParentItem(m_pSinglePlayerMenu);

    m_pButtonSinglePlayerBack = new MenuButton(tr("Back"));
    m_pButtonSinglePlayerBack->setSize(buttonWidth, buttonHeigth);
    m_pButtonSinglePlayerBack->setPos(width, height += buttonHeigth);
    m_pButtonSinglePlayerBack->setParentItem(m_pSinglePlayerMenu);

    connect(m_pButtonSelectDifficulty, &MenuSelectedButton::changeValue,
            m_pBattleShipCore, &BattleShipCore::changeBotDifficulty);
    connect(m_pButtonRandomPlaceMenu, &MenuButton::clicked, this, &BattleShipView::drawRandomPlaceMenu);
    connect(m_pButtonManualPlaceMenu, &MenuButton::clicked, this, &BattleShipView::drawManualPlaceMenu);
    connect(m_pButtonSinglePlayerBack, &MenuButton::clicked, this, &BattleShipView::drawMainMenu);

    return m_pSinglePlayerMenu;
}

QGraphicsRectItem *BattleShipView::initRandomPlaceMenu()
{
    auto countElements = 3;
    auto buttonWidth = m_pScene->width() / 3;
    auto buttonHeigth = m_pScene->height() / 8;
    auto width = m_pScene->width() / 4 - buttonWidth / 2;
    auto height = m_pScene->height() / 2 - ((countElements + 2) * buttonHeigth) / 2;

    m_pRandomPlaceMenu = new QGraphicsRectItem();
    m_pRandomPlaceMenu->setVisible(false);

    m_pRandomPlaceTitle = new TextLabel(tr("Random placement"));
    m_pRandomPlaceTitle->setSize(m_pScene->width(), buttonHeigth);
    m_pRandomPlaceTitle->setPos(0, height - 50);
    m_pRandomPlaceTitle->setParentItem(m_pRandomPlaceMenu);

    m_pButtonStartGameRPMenu = new MenuButton(tr("Start"));
    m_pButtonStartGameRPMenu->setSize(buttonWidth, buttonHeigth);
    m_pButtonStartGameRPMenu->setPos(width, height += buttonHeigth);
    m_pButtonStartGameRPMenu->setParentItem(m_pRandomPlaceMenu);

    m_pButtonGenerate = new MenuButton(tr("Generate"));
    m_pButtonGenerate->setSize(buttonWidth, buttonHeigth);
    m_pButtonGenerate->setPos(width, height += buttonHeigth);
    m_pButtonGenerate->setParentItem(m_pRandomPlaceMenu);

    m_pButtonRandomPlaceBack = new MenuButton(tr("Back"));
    m_pButtonRandomPlaceBack->setSize(buttonWidth, buttonHeigth);
    m_pButtonRandomPlaceBack->setPos(width, height += buttonHeigth);
    m_pButtonRandomPlaceBack->setParentItem(m_pRandomPlaceMenu);

    connect(m_pButtonStartGameRPMenu, &MenuButton::clicked, this, &BattleShipView::drawGame);
    connect(m_pButtonGenerate, &MenuButton::clicked, m_pBattleShipCore, &BattleShipCore::generateHumanMap);
    connect(m_pButtonRandomPlaceBack, &MenuButton::clicked, this, &BattleShipView::exitFromPlaceRandomlyMenu);

    return m_pRandomPlaceMenu;
}

QGraphicsRectItem *BattleShipView::initManualPlaceMenu()
{
    auto countElements = 2;
    auto buttonWidth = m_pScene->width() / 4;
    auto buttonHeigth = m_pScene->height() / 8;
    auto width = m_pScene->width() / 4 - buttonWidth / 2;
    auto height = m_pScene->height() / 2 - ((countElements + 2) * buttonHeigth) / 2;

    m_pManualPlaceMenu = new QGraphicsRectItem();
    m_pManualPlaceMenu->setVisible(false);

    m_pManualPlaceTitle = new TextLabel(tr("Manual placement"));
    m_pManualPlaceTitle->setSize(m_pScene->width(), buttonHeigth);
    m_pManualPlaceTitle->setPos(0, height - 100);
    m_pManualPlaceTitle->setParentItem(m_pManualPlaceMenu);

    m_pButtonStartGameMPMenu = new MenuButton(tr("Start"));
    m_pButtonStartGameMPMenu->setSize(buttonWidth, buttonHeigth);
    m_pButtonStartGameMPMenu->setPos(width, height += buttonHeigth);
    m_pButtonStartGameMPMenu->setParentItem(m_pManualPlaceMenu);

    m_pButtonManualPlaceBack = new MenuButton(tr("Back"));
    m_pButtonManualPlaceBack->setSize(buttonWidth, buttonHeigth);
    m_pButtonManualPlaceBack->setPos(width, height += buttonHeigth);
    m_pButtonManualPlaceBack->setParentItem(m_pManualPlaceMenu);

    auto map = m_pBattleShipCore->dragAndDropMap();
    map->setPos(m_pScene->width() / 2, m_pScene->height() / 2 - 165);
    map->setParentItem(m_pManualPlaceMenu);

    connect(m_pButtonStartGameMPMenu, &MenuButton::clicked, this, &BattleShipView::drawGameFromMenualPlace);
    connect(m_pButtonManualPlaceBack, &MenuButton::clicked, this, &BattleShipView::exitFromPlaceRandomlyMenu);

    return m_pManualPlaceMenu;
}

QGraphicsRectItem *BattleShipView::initGameMenu()
{
    auto width = m_pScene->width() / 2;
    auto height = m_pScene->height() / 2;

    m_pGameMenu = new QGraphicsRectItem();
    m_pGameMenu->setVisible(false);

    auto indicator = m_pBattleShipCore->turnIndicator();
    indicator->setPos(width - 30, height - 30);
    indicator->setParentItem(m_pGameMenu);

    m_pButtonGameMenuBack = new MenuButton(tr("Back"));
    m_pButtonGameMenuBack->setSize(100, 60);
    m_pButtonGameMenuBack->setPos(0, m_pScene->height() - 60);
    m_pButtonGameMenuBack->setParentItem(m_pGameMenu);

    connect(m_pButtonGameMenuBack, &MenuButton::clicked, this, &BattleShipView::exitFromGameToMenu);

    return m_pGameMenu;
}
