#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtWidgets>

const int MAP_SIZE = 10;

enum e_ButtonType {
    Standart,
    Disable
};
enum e_Status {
    Empty,
    NearbyShip,
    Life,
    Hit,
    Destroyed,
    Miss,
    Impossible
};
enum e_Direction {
    Down,
    Up,
    Left,
    Right,
    None
};

enum e_Difficulty {
    Easy,
    Medium,
    Hard
};

class Settings
{
public:
    static Settings &instance();
    void load();
    void save();
    QVector<int> shipsLength();
    QRectF sceneSize() const;
    QSize viewSize() const;
private:
    Settings();
    Settings(const Settings &other) = delete;
    Settings& operator=(const Settings &other) = delete;
};

#endif // SETTINGS_H
