#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtWidgets>

class Settings
{
public:
    static Settings &instance();
    void load();
    void save();
    QRectF sceneSize() const;
private:
    Settings();
    Settings(const Settings &other) = delete;
    Settings& operator=(const Settings &other) = delete;
};

#endif // SETTINGS_H
