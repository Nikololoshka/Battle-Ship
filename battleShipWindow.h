#ifndef BATTLESHIPWINDOW_H
#define BATTLESHIPWINDOW_H

#include <QtWidgets>
#include "settings.h"
#include "battleShip.h"
#include "battleShipNamespace.h"

class BattleShipWindow : public QMainWindow
{
    Q_OBJECT
public:
    BattleShipWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    BattleShip *view;
};

#endif // BATTLESHIPWINDOW_H
