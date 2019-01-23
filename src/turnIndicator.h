#ifndef TURNINDICATOR_H
#define TURNINDICATOR_H

#include <QtWidgets>
#include "settings.h"

class TurnIndicator : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
public:
    TurnIndicator(int width, int height);
    void setAnimationDelay(int msec);
    void reset();
    QColor color() const;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public slots:
    void change(double angle, QColor color);
    void setColor(QColor color);

signals:
    void colorChanged(QColor color);

private:
    QVector<QPointF> m_points;
    int m_width;
    int m_height;
    qreal m_angle;
    QColor m_color;

    QVariantAnimation m_directionAnimation;
    QPropertyAnimation m_colorAnimation;
};

#endif // TURNINDICATOR_H
