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
    QVector<int> temp = {1, 1, 1, 1, 2, 2, 2, 3, 3, 4};
    return temp;
}

QRectF Settings::sceneSize() const
{
    return QRectF(0, 0, 800, 600);
}

Settings::Settings()
{

}
