#include "settings.h"

Settings &Settings::instance()
{
    static Settings single;
    return single;
}

void Settings::load()
{

}

void Settings::save()
{

}

QRectF Settings::sceneSize() const
{
    return QRectF(0, 0, 800, 600);
}

Settings::Settings()
{

}
