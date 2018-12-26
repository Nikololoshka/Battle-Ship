#ifndef BATTLESHIPWINDOW_H
#define BATTLESHIPWINDOW_H

#include <QtWidgets>
#include "settings.h"
#include "battleShipView.h"

class BattleShipWindow : public QMainWindow
{
    Q_OBJECT
public:
    BattleShipWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    BattleShip *m_pView;
};

#endif // BATTLESHIPWINDOW_H
