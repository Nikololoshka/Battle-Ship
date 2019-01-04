#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtWidgets>
#include "ship.h"

class Settings
{
public:
    static Settings &inst();
    void load();
    void save();
    QVector<Ship *> standartShips();
    QRectF sceneSize() const;
    QSize viewSize() const;
    int animationDelay();
private:
    Settings();
    Settings(const Settings &other) = delete;
    Settings& operator=(const Settings &other) = delete;
};

#endif // SETTINGS_H
