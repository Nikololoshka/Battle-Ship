#include "battleShipView.h"

BattleShipView::BattleShipView(QWidget *parent)
    : QGraphicsView(parent)
{
    // initialization
    m_pScene = new QGraphicsScene(Settings::inst().sceneRect());
    m_pBattleShipCore = new BattleShipCore(this);

    m_pScene->addItem(initMainMenu());
    m_pScene->addItem(initSinglePlayerMenu());
    m_pScene->addItem(initGameMenu());    

    // settings
    m_pScene->setBackgroundBrush(QImage(QStringLiteral(":/image/bg-image")));

    setScene(m_pScene);
    setMinimumSize(800, 600);
    setRenderHint(QPainter::Antialiasing, true);
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    mainMenu.m_pMainMenu->setVisible(true);

    connect(m_pBattleShipCore, &BattleShipCore::endGame, this, &BattleShipView::playerWins);
}

BattleShipCore *BattleShipView::core() const
{
    return m_pBattleShipCore;
}

void BattleShipView::drawMainMenu(QGraphicsItem *clickedItem)
{
    clickedItem->parentItem()->setVisible(false);
    mainMenu.m_pMainMenu->setVisible(true);
}

void BattleShipView::drawSinglePlayerMenu(QGraphicsItem *clickedItem)
{
    clickedItem->parentItem()->setVisible(false);
    singlePlayerMenu.m_pSinglePlayerMenu->setVisible(true);

    clickedSetRandomMode();
}

void BattleShipView::drawGame(QGraphicsItem *clickedItem)
{
    if (!singlePlayerMenu.m_pGameMapEditor->isReady()) {
        QMessageBox::information(this, tr("Information"), tr("Not all ships are set up!"));
        return;
    }
    clickedItem->parentItem()->setVisible(false);
    gameMenu.m_pGameMenu->setVisible(true);

    m_pBattleShipCore->setShipsFromEditorToPlayer();
    m_pBattleShipCore->generateBotMap();
}

void BattleShipView::clickedSetClearMode()
{
    singlePlayerMenu.m_pSinglePlayerMenu->setX(-90);
    singlePlayerMenu.m_pSinglePlayerTitle->setX(90);
    singlePlayerMenu.m_pGameMapEditor->setClearMapMode();
    singlePlayerMenu. m_pGameMapEditor->update();
}

void BattleShipView::clickedSetRandomMode()
{
    singlePlayerMenu.m_pSinglePlayerMenu->setX(0);
    singlePlayerMenu.m_pSinglePlayerTitle->setX(0);
    singlePlayerMenu.m_pGameMapEditor->setGenerateRandomMode();
    singlePlayerMenu.m_pGameMapEditor->update();
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
    drawMainMenu(clickedItem);
}

void BattleShipView::playerWins(QString winnerName)
{
    QMessageBox::information(this, tr("Win!"), tr("Player ") + winnerName + tr(" is winner!"));

    exitFromGameToMenu(gameMenu.m_pGameMenu->childItems().first());
}

void BattleShipView::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        // main menu update
        mainMenu.m_pMainMenuTitle->setText(QApplication::applicationDisplayName());
        mainMenu.m_pButtonSinglePlayer->setText(tr("Singleplayer"));
        mainMenu.m_pButtonMultiPlayer->setText(tr("Multiplayer"));
        mainMenu.m_pButtonStatistics->setText(tr("Statistics"));
        mainMenu.m_pButtonExit->setText(tr("Exit"));

        // singleplayer menu update
        singlePlayerMenu.m_pSinglePlayerTitle->setText(tr("Singleplayer"));
        singlePlayerMenu.m_pButtonStartGame->setText(tr("Start game"));
        singlePlayerMenu.m_pButtonSelectDifficulty->setPrefix(tr("Diffeculty"));
        singlePlayerMenu.m_pButtonSelectDifficulty->updateTranslate(tr("Easy"), Bot::Easy);
        singlePlayerMenu.m_pButtonSelectDifficulty->updateTranslate(tr("Medium"), Bot::Medium);
        singlePlayerMenu.m_pButtonClearMap->setText(tr("Clear map"));
        singlePlayerMenu.m_pButtonGenerateNewMap->setText(tr("Generate new map"));
        singlePlayerMenu.m_pGameMapEditor->setLabelText(tr("ABCDEFGHIJ"));
        singlePlayerMenu.m_pButtonSinglePlayerBack->setText(tr("Back"));

        // game menu update
        gameMenu.m_pHumanMap->setLabelText(tr("ABCDEFGHIJ"));
        gameMenu.m_pBotMap->setLabelText(tr("ABCDEFGHIJ"));
        gameMenu.m_pButtonGameMenuBack->setText(tr("Back"));
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

    mainMenu.m_pMainMenu = new QGraphicsRectItem();
    mainMenu.m_pMainMenu->setVisible(false);

    mainMenu.m_pMainMenuTitle = new TextLabel(QApplication::applicationDisplayName());
    mainMenu.m_pMainMenuTitle->setSize(m_pScene->width(), buttonHeigth);
    mainMenu.m_pMainMenuTitle->setPos(0, height - 50);
    mainMenu.m_pMainMenuTitle->setParentItem(mainMenu.m_pMainMenu);

    mainMenu.m_pButtonSinglePlayer = new MenuButton(tr("Singleplayer"));
    mainMenu.m_pButtonSinglePlayer->setSize(buttonWidth, buttonHeigth);
    mainMenu.m_pButtonSinglePlayer->setPos(width, height += buttonHeigth);
    mainMenu.m_pButtonSinglePlayer->setParentItem(mainMenu.m_pMainMenu);

    mainMenu.m_pButtonMultiPlayer = new MenuDisableButton(tr("Multiplayer"));
    mainMenu.m_pButtonMultiPlayer->setSize(buttonWidth, buttonHeigth);
    mainMenu.m_pButtonMultiPlayer->setPos(width, height += buttonHeigth);
    mainMenu.m_pButtonMultiPlayer->setParentItem(mainMenu.m_pMainMenu);

    mainMenu.m_pButtonStatistics = new MenuDisableButton(tr("Statistics"));
    mainMenu.m_pButtonStatistics->setSize(buttonWidth, buttonHeigth);
    mainMenu.m_pButtonStatistics->setPos(width, height += buttonHeigth);
    mainMenu.m_pButtonStatistics->setParentItem(mainMenu.m_pMainMenu);

    mainMenu.m_pButtonExit = new MenuButton(tr("Exit"));
    mainMenu.m_pButtonExit->setSize(buttonWidth, buttonHeigth);
    mainMenu.m_pButtonExit->setPos(width, height += buttonHeigth);
    mainMenu.m_pButtonExit->setParentItem(mainMenu.m_pMainMenu);

    connect(mainMenu.m_pButtonSinglePlayer, &MenuButton::clicked, this, &BattleShipView::drawSinglePlayerMenu);
    connect(mainMenu.m_pButtonExit, &MenuButton::clicked, this->parentWidget(), &QWidget::close);

    return mainMenu.m_pMainMenu;
}

QGraphicsRectItem *BattleShipView::initSinglePlayerMenu()
{
    auto countElements = 5;
    auto buttonWidth = m_pScene->width() / 3;
    auto buttonHeigth = m_pScene->height() / 10;
    auto width = m_pScene->width() / 4 - buttonWidth / 2 + 20;
    auto height = m_pScene->height() / 2 - ((countElements + 2) * buttonHeigth) / 2;

    singlePlayerMenu.m_pSinglePlayerMenu = new QGraphicsRectItem();
    singlePlayerMenu.m_pSinglePlayerMenu->setVisible(false);

    singlePlayerMenu.m_pSinglePlayerTitle = new TextLabel(tr("Singleplayer"));
    singlePlayerMenu.m_pSinglePlayerTitle->setSize(m_pScene->width(), buttonHeigth);
    singlePlayerMenu.m_pSinglePlayerTitle->setPos(0, height - 50);
    singlePlayerMenu.m_pSinglePlayerTitle->setParentItem(singlePlayerMenu.m_pSinglePlayerMenu);

    singlePlayerMenu.m_pButtonStartGame = new MenuButton(tr("Start game"));
    singlePlayerMenu.m_pButtonStartGame->setSize(buttonWidth, buttonHeigth);
    singlePlayerMenu.m_pButtonStartGame->setPos(width, height += buttonHeigth);
    singlePlayerMenu.m_pButtonStartGame->setParentItem(singlePlayerMenu.m_pSinglePlayerMenu);

    singlePlayerMenu.m_pButtonSelectDifficulty = new MenuSelectedButton(buttonWidth, buttonHeigth);
    singlePlayerMenu.m_pButtonSelectDifficulty->setPrefix(tr("Diffeculty"));
    singlePlayerMenu.m_pButtonSelectDifficulty->addOption(tr("Easy"), Bot::Easy);
    singlePlayerMenu.m_pButtonSelectDifficulty->addOption(tr("Medium"), Bot::Medium);
    singlePlayerMenu.m_pButtonSelectDifficulty->setPos(width, height += buttonHeigth);
    singlePlayerMenu.m_pButtonSelectDifficulty->setParentItem(singlePlayerMenu.m_pSinglePlayerMenu);

    singlePlayerMenu.m_pButtonClearMap = new MenuButton(tr("Clear map"));
    singlePlayerMenu.m_pButtonClearMap->setSize(buttonWidth, buttonHeigth);
    singlePlayerMenu.m_pButtonClearMap->setPos(width, height += buttonHeigth);
    singlePlayerMenu.m_pButtonClearMap->setParentItem(singlePlayerMenu.m_pSinglePlayerMenu);

    singlePlayerMenu.m_pButtonGenerateNewMap = new MenuButton(tr("Generate new map"));
    singlePlayerMenu.m_pButtonGenerateNewMap->setSize(buttonWidth, buttonHeigth);
    singlePlayerMenu.m_pButtonGenerateNewMap->setPos(width, height += buttonHeigth);
    singlePlayerMenu.m_pButtonGenerateNewMap->setParentItem(singlePlayerMenu.m_pSinglePlayerMenu);

    singlePlayerMenu.m_pButtonSinglePlayerBack = new MenuButton(tr("Back"));
    singlePlayerMenu.m_pButtonSinglePlayerBack->setSize(buttonWidth, buttonHeigth);
    singlePlayerMenu.m_pButtonSinglePlayerBack->setPos(width, height += buttonHeigth);
    singlePlayerMenu.m_pButtonSinglePlayerBack->setParentItem(singlePlayerMenu.m_pSinglePlayerMenu);

    singlePlayerMenu.m_pGameMapEditor = m_pBattleShipCore->gameMapEditor();
    singlePlayerMenu.m_pGameMapEditor->setLabelText(tr("ABCDEFGHIJ"));
    singlePlayerMenu.m_pGameMapEditor->setPos(m_pScene->width() / 2 - 20, m_pScene->height() / 2 - 165);
    singlePlayerMenu.m_pGameMapEditor->setParentItem(singlePlayerMenu.m_pSinglePlayerMenu);

    connect(singlePlayerMenu.m_pButtonStartGame, &MenuButton::clicked,
            this, &BattleShipView::drawGame);
    connect(singlePlayerMenu.m_pButtonSelectDifficulty, &MenuSelectedButton::changeValue,
            m_pBattleShipCore, &BattleShipCore::changeBotDifficulty);
    connect(singlePlayerMenu.m_pButtonClearMap, &MenuButton::clicked,
            this, &BattleShipView::clickedSetClearMode);
    connect(singlePlayerMenu.m_pButtonGenerateNewMap, &MenuButton::clicked,
            this, &BattleShipView::clickedSetRandomMode);
    connect(singlePlayerMenu.m_pButtonSinglePlayerBack, &MenuButton::clicked,
            this, &BattleShipView::drawMainMenu);

    return singlePlayerMenu.m_pSinglePlayerMenu;
}

QGraphicsRectItem *BattleShipView::initGameMenu()
{
    auto width = m_pScene->width() / 2;
    auto height = m_pScene->height() / 2;

    gameMenu.m_pGameMenu = new QGraphicsRectItem();
    gameMenu.m_pGameMenu->setVisible(false);

    gameMenu.m_pHumanMap = m_pBattleShipCore->playerHumanMap();
    gameMenu.m_pHumanMap->setLabelText(tr("ABCDEFGHIJ"));
    gameMenu.m_pHumanMap->setPos(m_pScene->width() / 2 - 380, m_pScene->height() / 2 - 180);
    gameMenu.m_pHumanMap->setParentItem(gameMenu.m_pGameMenu);

    gameMenu.m_pBotMap = m_pBattleShipCore->playerBotMap();
    gameMenu.m_pBotMap->setLabelText(tr("ABCDEFGHIJ"));
    gameMenu.m_pBotMap->setPos(m_pScene->width() / 2 + 50, m_pScene->height() / 2 - 180);
    gameMenu.m_pBotMap->setParentItem(gameMenu.m_pGameMenu);

    auto indicator = m_pBattleShipCore->turnIndicator();
    indicator->setPos(width - 30, height - 30);
    indicator->setParentItem(gameMenu.m_pGameMenu);

    gameMenu.m_pButtonGameMenuBack = new MenuButton(tr("Back"));
    gameMenu.m_pButtonGameMenuBack->setSize(100, 60);
    gameMenu.m_pButtonGameMenuBack->setPos(0, m_pScene->height() - 60);
    gameMenu.m_pButtonGameMenuBack->setParentItem(gameMenu.m_pGameMenu);

    connect(gameMenu.m_pButtonGameMenuBack, &MenuButton::clicked, this, &BattleShipView::exitFromGameToMenu);

    return gameMenu.m_pGameMenu;
}
