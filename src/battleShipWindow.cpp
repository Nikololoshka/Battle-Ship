#include "battleShipWindow.h"

BattleShipWindow::BattleShipWindow(QWidget *parent)
    : QMainWindow (parent)
{
    // initialization
    m_pView = new BattleShipView(this);

    // setting
    QMenu *menuFile = new QMenu(tr("&File"));
    menuFile->addAction(tr("Exit"), this, &BattleShipWindow::close, QKeySequence("CTRL+Q"));
    menuBar()->addMenu(menuFile);

    QMenu *menuAbout = new QMenu(tr("&About"));
    menuAbout->addAction(tr("About battle ship"), [this](){
        QMessageBox::information(this, tr("About battle ship"),
                                 tr("Create by:\n"
                                    "Nikolay Vereshchagin\n"
                                    "Stankin, IDB-17-09\n\n"
                                    "Thank you for materials:\n") +
                                    "subtlepatterns.com - https://subtlepatterns.com/\n");
    });
    menuBar()->addMenu(menuAbout);

    // layout setup
    setCentralWidget(m_pView);
}

void BattleShipWindow::closeEvent(QCloseEvent *event)
{
    if (m_pView->core()->isChange()) {
        auto answer = QMessageBox::question(this, tr("Exit"), tr("Are you sure you want to go out?"));

        if (answer == QMessageBox::StandardButton::No) {
            event->ignore();
            return;
        }
    }
    QWidget::closeEvent(event);
}
