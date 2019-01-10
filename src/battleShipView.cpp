#include "battleShipView.h"

BattleShipView::BattleShipView(QWidget *parent)
    : QGraphicsView(parent)
{
    // initialization
    m_pScene = new QGraphicsScene(Settings::inst().sceneSize());
    m_pBattleShipCore = new BattleShipCore(this);
    m_pScene->addItem(m_pBattleShipCore->playerHumanMap());
    m_pScene->addItem(m_pBattleShipCore->playerBotMap());

    m_pScene->addItem(initMainMenu());
    m_pScene->addItem(initSinglePlayerMenu());
    m_pScene->addItem(initRandomPlaceMenu());
    m_pScene->addItem(initManualPlaceMenu());
    m_pScene->addItem(initGameMenu());    

    // settings
    m_pScene->setBackgroundBrush(QImage(QStringLiteral(":/bg-image.png")));

    setScene(m_pScene);
    setMinimumSize(900, 600);
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
    m_pPlaceRandomlyMenu->setVisible(true);

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
        auto answer = QMessageBox::question(this, tr("Exit"), tr("Are you sure you want to go out?"));

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

QGraphicsRectItem *BattleShipView::initMainMenu()
{
    auto countElements = 4;
    auto buttonWidth = m_pScene->width() / 3;
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

    MenuDisableButton *buttonMultiPlayer = new MenuDisableButton(tr("Multiplayer"));
    buttonMultiPlayer->setSize(buttonWidth, buttonHeigth);
    buttonMultiPlayer->setPos(width, height += buttonHeigth);
    buttonMultiPlayer->setParentItem(m_pMainMenu);

    MenuDisableButton *buttonStatistics = new MenuDisableButton(tr("Statistics"));
    buttonStatistics->setSize(buttonWidth, buttonHeigth);
    buttonStatistics->setPos(width, height += buttonHeigth);
    buttonStatistics->setParentItem(m_pMainMenu);

    MenuButton *buttonExit = new MenuButton(tr("Exit"));
    buttonExit->setSize(buttonWidth, buttonHeigth);
    buttonExit->setPos(width, height += buttonHeigth);
    buttonExit->setParentItem(m_pMainMenu);

    connect(buttonSinglePlayer, &MenuButton::clicked, this, &BattleShipView::drawSinglePlayerMenu);
    connect(buttonExit, &MenuButton::clicked, this->parentWidget(), &QWidget::close);

    return m_pMainMenu;
}

QGraphicsRectItem *BattleShipView::initSinglePlayerMenu()
{
    auto countElements = 4;
    auto buttonWidth = m_pScene->width() / 3;
    auto buttonHeigth = m_pScene->height() / 7;
    auto width = m_pScene->width() / 2 - buttonWidth / 2;
    auto height = m_pScene->height() / 2 - ((countElements + 2) * buttonHeigth) / 2;

    m_pSinglePlayerMenu = new QGraphicsRectItem();
    m_pSinglePlayerMenu->setVisible(false);

    TextLabel *titleLabel = new TextLabel(tr("Singleplayer"));
    titleLabel->setSize(m_pScene->width(), buttonHeigth);
    titleLabel->setPos(0, height);
    titleLabel->setParentItem(m_pSinglePlayerMenu);

    MenuSelectedButton *buttonSelectDifficulty = new MenuSelectedButton(buttonWidth, buttonHeigth);
    buttonSelectDifficulty->addOption(tr("Easy"), Bot::Easy);
    buttonSelectDifficulty->addOption(tr("Medium"), Bot::Medium);
    buttonSelectDifficulty->setPos(width, height += buttonHeigth);
    buttonSelectDifficulty->setParentItem(m_pSinglePlayerMenu);

    MenuButton *buttonRandomPlaceMenu = new MenuButton(tr("Random"));
    buttonRandomPlaceMenu->setSize(buttonWidth, buttonHeigth);
    buttonRandomPlaceMenu->setPos(width, height += buttonHeigth);
    buttonRandomPlaceMenu->setParentItem(m_pSinglePlayerMenu);

    MenuButton *buttonManualPlaceMenu = new MenuButton(tr("Manual"));
    buttonManualPlaceMenu->setSize(buttonWidth, buttonHeigth);
    buttonManualPlaceMenu->setPos(width, height += buttonHeigth);
    buttonManualPlaceMenu->setParentItem(m_pSinglePlayerMenu);

    MenuButton *buttonSinglePlayerBack = new MenuButton(tr("Back"));
    buttonSinglePlayerBack->setSize(buttonWidth, buttonHeigth);
    buttonSinglePlayerBack->setPos(width, height += buttonHeigth);
    buttonSinglePlayerBack->setParentItem(m_pSinglePlayerMenu);

    connect(buttonSelectDifficulty, &MenuSelectedButton::changeValue,
            m_pBattleShipCore, &BattleShipCore::changeBotDifficulty);
    connect(buttonRandomPlaceMenu, &MenuButton::clicked, this, &BattleShipView::drawRandomPlaceMenu);
    connect(buttonManualPlaceMenu, &MenuButton::clicked, this, &BattleShipView::drawManualPlaceMenu);
    connect(buttonSinglePlayerBack, &MenuButton::clicked, this, &BattleShipView::drawMainMenu);

    return m_pSinglePlayerMenu;
}

QGraphicsRectItem *BattleShipView::initRandomPlaceMenu()
{
    auto countElements = 3;
    auto buttonWidth = m_pScene->width() / 3;
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

    connect(buttonStartGame, &MenuButton::clicked, this, &BattleShipView::drawGame);
    connect(buttonGenerate, &MenuButton::clicked, m_pBattleShipCore, &BattleShipCore::generateHumanMap);
    connect(buttonPlaceRandomlyBack, &MenuButton::clicked, this, &BattleShipView::exitFromPlaceRandomlyMenu);

    return m_pPlaceRandomlyMenu;
}

QGraphicsRectItem *BattleShipView::initManualPlaceMenu()
{
    auto countElements = 2;
    auto buttonWidth = m_pScene->width() / 4;
    auto buttonHeigth = m_pScene->height() / 7;
    auto width = m_pScene->width() / 4 - buttonWidth / 2;
    auto height = m_pScene->height() / 2 - ((countElements + 2) * buttonHeigth) / 2;

    m_pManualPlaceMenu = new QGraphicsRectItem();
    m_pManualPlaceMenu->setVisible(false);

    TextLabel *titleLabel = new TextLabel(tr("Manual placement"));
    titleLabel->setSize(m_pScene->width(), buttonHeigth);
    titleLabel->setPos(0, height - 100);
    titleLabel->setParentItem(m_pManualPlaceMenu);

    MenuButton *buttonStartGame = new MenuButton(tr("Start"));
    buttonStartGame->setSize(buttonWidth, buttonHeigth);
    buttonStartGame->setPos(width, height += buttonHeigth);
    buttonStartGame->setParentItem(m_pManualPlaceMenu);

    MenuButton *buttonPlaceRandomlyBack = new MenuButton(tr("Back"));
    buttonPlaceRandomlyBack->setSize(buttonWidth, buttonHeigth);
    buttonPlaceRandomlyBack->setPos(width, height += buttonHeigth);
    buttonPlaceRandomlyBack->setParentItem(m_pManualPlaceMenu);

    auto map = m_pBattleShipCore->dragAndDropMap();
    map->setPos(m_pScene->width() / 2, m_pScene->height() / 2 - 165);
    map->setParentItem(m_pManualPlaceMenu);

    connect(buttonStartGame, &MenuButton::clicked, this, &BattleShipView::drawGameFromMenualPlace);
    connect(buttonPlaceRandomlyBack, &MenuButton::clicked, this, &BattleShipView::exitFromPlaceRandomlyMenu);

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

    MenuButton *buttonBack = new MenuButton(tr("Back"));
    buttonBack->setSize(100, 60);
    buttonBack->setPos(0, m_pScene->height() - 60);
    buttonBack->setParentItem(m_pGameMenu);

    connect(buttonBack, &MenuButton::clicked, this, &BattleShipView::exitFromGameToMenu);

    return m_pGameMenu;
}
