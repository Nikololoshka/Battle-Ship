#ifndef APPNAMESPACE_H
#define APPNAMESPACE_H

#include <QtWidgets>
#include <random>

const int g_MAP_SIZE = 10;

enum class e_Status {
    Empty,
    NearbyShip,
    Life,
    Hit,
    Destroyed,
    Miss,
    Impossible
};
enum class e_Direction {
    Down,
    Up,
    Left,
    Right,
    None
};

QFont computeFontForText(QRect rect, int flags, const QString &text, double scale = 0.65);
int generateRandomNumber(int from, int to);


#endif // APPNAMESPACE_H
