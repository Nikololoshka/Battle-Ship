#include "grahicelements.h"


MenuButton::MenuButton(const QString &title, double x, double y, double width, double heigth)
    : QGraphicsObject (),
      title_(title), x_(x), y_(y), width_(width), heigth_(heigth)
{
}


QRectF MenuButton::boundingRect() const
{
    return QRectF(0, 0, width_, heigth_);
}

void MenuButton::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->setPen(Qt::black);
    painter->drawRect(this->boundingRect());
    painter->drawText(this->boundingRect(), Qt::AlignCenter, title_);
    qDebug() << "paint";
}


void MenuButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked();
    QGraphicsItem::mouseReleaseEvent(event);
}
