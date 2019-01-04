#ifndef TURNINDICATOR_H
#define TURNINDICATOR_H

#include <QtWidgets>
#include "settings.h"

class TurnIndicator : public QGraphicsObject
{
    Q_OBJECT
public:
    TurnIndicator(int width, int height);
    void setDiraction(e_Direction dir);
    void setColor(Qt::GlobalColor color);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public slots:
    void change(e_Direction dir, Qt::GlobalColor color);

private:
    QVector<QPointF> m_points;
    int m_width;
    int m_height;
    qreal m_angle;
    Qt::GlobalColor m_color;
    e_Direction m_dir;
};

#endif // TURNINDICATOR_H
