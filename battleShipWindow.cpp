#include "battleShipWindow.h"

BattleShipWindow::BattleShipWindow(QWidget *parent)
    : QMainWindow (parent)
{
    // initialization
    Settings::instance().load();

    m_pView = new BattleShip(this);

    // setting
    QMenu *menuFile = new QMenu(tr("&File"));
    menuFile->addAction(tr("Exit"), this, &BattleShipWindow::close, QKeySequence("CTRL+Q"));
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
    setCentralWidget(m_pView);
}

void BattleShipWindow::closeEvent(QCloseEvent *event)
{
    if (m_pView->isChange()) {
        auto answer = QMessageBox::question(this, tr("Exit"), tr("Are you sure you want to go out?"));

        if (answer == QMessageBox::StandardButton::No) {
            event->ignore();
            return;
        }
    }
    Settings::instance().save();
    QWidget::closeEvent(event);
}

