#ifndef APPNAMESPACE_H
#define APPNAMESPACE_H

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

#endif // APPNAMESPACE_H
