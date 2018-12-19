#include "battleShipWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BattleShipWindow w;
    w.show();

    return a.exec();
}
