#include "battleShipWindow.h"

BattleShipWindow::BattleShipWindow(QWidget *parent)
    : QMainWindow (parent)
{
    QApplication::setApplicationDisplayName(tr("Battle Ship"));

    // translation setup
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    QApplication::installTranslator(&qtTranslator);

    QTranslator *appTranslator = new QTranslator;
    appTranslator->load(":/translations/language_" + Settings::inst().language());
    QApplication::installTranslator(appTranslator);
    Settings::inst().setTranslator(appTranslator);

    // initialization
    m_pView = new BattleShipView(this);

    // setting
    setWindowTitle(QApplication::applicationDisplayName());
    setMinimumSize(840, 630);
    resize(Settings::inst().windowSize());

    m_pMenuGame = new QMenu(tr("&Game"));
    m_pSettingsMenuGame = m_pMenuGame->addAction(tr("Settings"), this,
                                                 &BattleShipWindow::settingWindow);
    m_pAboutMenuGame = m_pMenuGame->addAction(tr("About"), this,
                                              &BattleShipWindow::aboutBattleShip);
    m_pExitMenuGame = m_pMenuGame->addAction(tr("Exit"), this,
                                             &BattleShipWindow::close, QKeySequence("CTRL+Q"));
    menuBar()->addMenu(m_pMenuGame);

    // layout setup
    setCentralWidget(m_pView);
}

void BattleShipWindow::closeEvent(QCloseEvent *event)
{
    if (m_pView->core()->isChange()) {
        auto answer = QMessageBox::question(this, tr("Exit from program"),
                                            tr("Are you sure you want to go out?",
                                               "From the program during the game"));

        if (answer == QMessageBox::No) {
            event->ignore();
            return;
        }
    }
    Settings::inst().setWindowSize(this->size());
    QWidget::closeEvent(event);
}

void BattleShipWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        QApplication::setApplicationDisplayName(tr("Battle Ship"));
        setWindowTitle(QApplication::applicationDisplayName());

        m_pMenuGame->setTitle(tr("&Game"));
        m_pSettingsMenuGame->setText(tr("Settings"));
        m_pAboutMenuGame->setText(tr("About"));
        m_pExitMenuGame->setText(tr("Exit"));
    } else {
        QWidget::changeEvent(event);
    }
}

bool BattleShipWindow::event(QEvent *event)
{
    if (event->type() == SettingsChangeEvent::typeEvent()) {
        return QApplication::sendEvent(m_pView, event);
    }
    return QMainWindow::event(event);
}

void BattleShipWindow::aboutBattleShip()
{
    QMessageBox::information(this, tr("About battle ship"),
                             tr("Create by:\n"
                                "Nikolay Vereshchagin\n"
                                "Stankin, IDB-17-09\n\n"
                                "Thank you for materials:\n") +
                             "subtlepatterns.com - https://subtlepatterns.com/\n");
}

void BattleShipWindow::settingWindow()
{
    SettingsWindow window(this);
    window.exec();
}

