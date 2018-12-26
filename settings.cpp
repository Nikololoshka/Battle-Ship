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

QVector<int> Settings::shipsLength()
{
    QVector<int> temp = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    return temp;
}

QRectF Settings::sceneSize() const
{
    return QRectF(0, 0, 800, 600);
}

Settings::Settings()
{

}
