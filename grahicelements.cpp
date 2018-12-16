#include "grahicelements.h"


MenuButton::MenuButton(const QString &title, double width, double heigth, ButtonType type)
    : QGraphicsObject (),
      title_(title), width_(width), heigth_(heigth), hover_(false), type_(type)
{
    setAcceptHoverEvents(true);

    switch (type_) {
    case ButtonType::Disable:
        setEnabled(false);
        break;
    default:
        break;
    }
}

void MenuButton::reset()
{
    hover_ = false;
}


QRectF MenuButton::boundingRect() const
{
    return QRectF(0, 0, width_, heigth_);
}

void MenuButton::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    switch (type_) {
    case ButtonType::Disable:
    {
        QLinearGradient gradient(0, 0, width_, width_);
        for (int i = 0; i < 50; ++i) {
            if (i % 2 == 0)
                gradient.setColorAt(i / 50.0, Qt::gray);
            else
                gradient.setColorAt(i / 50.0, Qt::white);
        }
        painter->setBrush(gradient);
        break;
    }
    default:
        if (hover_)
            painter->setBrush(Qt::blue);
        else
            painter->setBrush(Qt::cyan);
        break;
    }

    painter->setPen(Qt::black);
    painter->setFont(QFont("Arial", 25));
    painter->drawRect(this->boundingRect());
    painter->drawText(this->boundingRect(), Qt::AlignCenter, title_);
}

void MenuButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked();
    QGraphicsItem::mousePressEvent(event);
}


void MenuButton::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    hover_ = true;
    update();
    QGraphicsItem::hoverMoveEvent(event);
}

void MenuButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    hover_ = false;
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}

//----------------------------------------------------------------------------------------------------------------------

Cell::Cell(double x, double y, int w, int h, int id, Status status)
    : width_(w), height_(h), id_(id), hover_(false), status_(status)
{
    if (status != Status::Disable)
        setAcceptHoverEvents(true);
    setPos(x, y);
}

void Cell::shot()
{
    status_ = Status::Miss;
    update();
}

QRectF Cell::boundingRect() const
{
    return  QRectF(0, 0, width_, height_);
}

void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{

    switch (status_) {
    case Status::Miss:
        painter->setBrush(Qt::cyan);
        painter->drawRect(this->boundingRect());

        painter->setPen(Qt::red);
        painter->drawLine(0, 0, width_, height_);
        painter->drawLine(0, height_, width_, 0);

        if (!hover_)
            painter->setPen(Qt::black);

        break;
    case Status::Destroyed:
        painter->setBrush(Qt::red);        
        painter->drawRect(this->boundingRect());

        break;
    default:
        if (hover_)            
            painter->setPen(Qt::magenta);
        else            
            painter->setPen(Qt::black);

        painter->setBrush(Qt::cyan);
        painter->drawRect(this->boundingRect());

        break;
    }
}

void Cell::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    hover_ = true;
    update();
    QGraphicsItem::hoverMoveEvent(event);
}

void Cell::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    hover_ = false;
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}

void Cell::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(id_);
    QGraphicsItem::mousePressEvent(event);
}
