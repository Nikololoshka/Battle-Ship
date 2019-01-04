#include "turnIndicator.h"

TurnIndicator::TurnIndicator(int width, int height)
    : QGraphicsObject (),
      m_width(width),
      m_height(height),
      m_color(Qt::green),
      m_dir(e_Direction::Right)
{
    setTransformOriginPoint(m_width / 2, m_height / 2);
    m_points = {
        {0, m_height / 3.0},
        {0, m_height * 2 / 3.0},
        {m_width * 2 / 3.0, m_height * 2 / 3.0},
        {m_width * 2 / 3.0, m_height / 1.0},
        {m_width / 1.0, m_height / 2.0},
        {m_width * 2 / 3.0, 0.0},
        {m_width * 2 / 3.0, m_height / 3.0},
    };
}

void TurnIndicator::setDiraction(e_Direction dir)
{
    m_dir = dir;
}

void TurnIndicator::setColor(Qt::GlobalColor color)
{
    m_color = color;
}

QRectF TurnIndicator::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void TurnIndicator::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->setPen(Qt::black);
    painter->setBrush(m_color);

//    switch (m_dir) {
//    case e_Direction::Right:
//        painter->rotate(0);
//        break;
//    case e_Direction::Left:
//        painter->translate(m_width, m_height);
//        painter->rotate(180);
//        break;
//    default:
//        break;
//    }
    painter->drawPolygon(m_points.constData(), m_points.size());
}

void TurnIndicator::change(e_Direction dir, Qt::GlobalColor color)
{
    m_dir = dir;

    QVariantAnimation *animDir = new QVariantAnimation;
//    QVariantAnimation *animColor = new QVariantAnimation;

    animDir->setDuration(Settings::inst().animationDelay() / 2);
//    animColor->setDuration(Settings::inst().animationDelay() / 2);

    animDir->setStartValue(m_angle);
//    animColor->setStartValue(QColor(m_color));

    animDir->setEndValue(m_angle + 180);
//    animColor->setEndValue(QColor(color));

    connect(animDir, &QVariantAnimation::valueChanged, [this](const QVariant &value){
        this->setRotation(value.toReal());
    });
//    connect(animColor, &QVariantAnimation::valueChanged, [this](const QVariant &value){
//        this->m_color = ;
//    });


    m_angle = static_cast<int>(m_angle + 180) % 360;
    m_color = color;
    animDir->start(QVariantAnimation::DeleteWhenStopped);
}

