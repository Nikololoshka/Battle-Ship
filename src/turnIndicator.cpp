#include "turnIndicator.h"

TurnIndicator::TurnIndicator(int width, int height)
    : QGraphicsObject (),
      m_width(width),
      m_height(height),
      m_angle(0),
      m_color(Qt::green)
{
    setTransformOriginPoint(m_width / 2, m_height / 2);

    m_colorAnimation.setTargetObject(this);
    m_colorAnimation.setPropertyName("color");
    m_colorAnimation.setDuration(Settings::inst().animationDelay() / 2);
    m_directionAnimation.setDuration(Settings::inst().animationDelay() / 2);

    m_points = {
        {0, m_height / 3.0},
        {0, m_height * 2 / 3.0},
        {m_width * 2 / 3.0, m_height * 2 / 3.0},
        {m_width * 2 / 3.0, m_height / 1.0},
        {m_width / 1.0, m_height / 2.0},
        {m_width * 2 / 3.0, 0.0},
        {m_width * 2 / 3.0, m_height / 3.0},
    };

    connect(&m_directionAnimation, &QVariantAnimation::valueChanged, [this](const QVariant &value){
        this->setRotation(value.toReal());
    });
}

void TurnIndicator::setAnimationDelay(int msec)
{
    m_colorAnimation.setDuration(msec / 2);
    m_directionAnimation.setDuration(msec / 2);
}

void TurnIndicator::reset()
{
    m_angle = 0;
    setRotation(m_angle);
    m_color = Qt::green;
    update();
}

QRectF TurnIndicator::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void TurnIndicator::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->setPen(Qt::black);
    painter->setBrush(m_color);
    painter->drawPolygon(m_points.constData(), m_points.size());
}

QColor TurnIndicator::color() const
{
    return m_color;
}

void TurnIndicator::change(double angle, QColor color)
{
    m_colorAnimation.setStartValue(m_color);
    m_directionAnimation.setStartValue(m_angle);

    m_colorAnimation.setEndValue(QColor(color));
    m_directionAnimation.setEndValue(m_angle + angle);

    m_angle = static_cast<int>(m_angle + angle) % 360;
    m_color = color;

    m_colorAnimation.start();
    m_directionAnimation.start();
}

void TurnIndicator::setColor(QColor color)
{
    if (m_color == color)
        return;

    m_color = color;
    emit colorChanged(m_color);
}

