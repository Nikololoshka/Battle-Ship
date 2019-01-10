#include "settings.h"

Settings &Settings::inst()
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

QVector<QSharedPointer<Ship> > Settings::standartShips()
{
    QVector<int> shipsLength = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    QVector<QSharedPointer<Ship>> result(shipsLength.size());

    for (int i = 0; i < shipsLength.size(); ++i)
        result[i] = QSharedPointer<Ship>::create(shipsLength[i]);

    return result;
}

QRectF Settings::sceneSize() const
{
    return QRectF(0, 0, 800, 600);
}

QSize Settings::viewSize() const
{
    return QSize(1000, 600);
}

int Settings::animationDelay()
{
    return 500;
}

Settings::Settings()
{

}
