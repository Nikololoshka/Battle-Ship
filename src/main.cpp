#include <QApplication>
#include "battleShipWindow.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    BattleShipWindow wgt;
    wgt.show();

    return app.exec();
}
