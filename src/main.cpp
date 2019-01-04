#include <QApplication>
#include "battleShipWindow.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Settings::inst().load();
    BattleShipWindow w;
    w.show();

    return a.exec();
}
