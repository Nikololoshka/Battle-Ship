#ifndef BATTLESHIPNAMESPACE_H
#define BATTLESHIPNAMESPACE_H


namespace App {
    enum ButtonType {
        Standart,
        Disable
    };
    enum Status {
        Empty,
        NearbyShip,
        Life,
        Hit,
        Destroyed,
        Miss,
        Impossible
    };
    enum Direction {
        Up,
        Down,
        Left,
        Right,
        None
    };
}

#endif // BATTLESHIPNAMESPACE_H
