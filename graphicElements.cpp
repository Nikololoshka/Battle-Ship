#include "graphicElements.h"

MenuButton::MenuButton(const QString &title, double width, double heigth, App::ButtonType type)
    : QGraphicsObject (),
      title_(title), width_(width), heigth_(heigth), hover_(false), type_(type)
{
    setAcceptHoverEvents(true);

    switch (type_) {
    case App::Disable:
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
    case App::Disable:
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

Cell::Cell(double x, double y, int w, int h, int id, bool disable)
    : width_(w), height_(h), id_(id), hover_(false),
      disable_(disable), status_(App::Empty), ship_(nullptr)
{
    if (!disable)
        setAcceptHoverEvents(true);
    else
        setAcceptedMouseButtons(Qt::MouseButton::NoButton);

    setPos(x, y);
}

App::Status Cell::shot(int x, int y)
{
    switch (status_) {
    case App::Empty:         // если пусто
    case App::NearbyShip:    // то промах
        status_ = App::Miss;
        update();
        return App::Miss;
    case App::Life:
        switch(ship_->shot(x, y)) {
        case App::Hit:         // если попали
            status_ = App::Hit;
            update();
            return  App::Hit;
        case App::Destroyed:   // если уничтожили
            status_ = App::Destroyed;
            update();
            return  App::Destroyed;
        default:                // если не попали
            return App::Impossible;   // что, в принципе, не возможно
        }
    default:                    // если повторно стреляем
        return App::Impossible; // туда же
    }
}

void Cell::setStatus(App::Status stat)
{
    status_ = stat;
    update();
}

void Cell::setShip(Ship *ship)
{
    ship_ = ship;
}

App::Status Cell::status() const
{
    return status_;
}

QRectF Cell::boundingRect() const
{
    return  QRectF(0, 0, width_, height_);
}

void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    switch (status_) {
    case App::Miss:
        painter->setBrush(Qt::cyan);
        painter->drawRect(0, 0, width_, height_);
        painter->setPen(QPen(Qt::red, 2));
        painter->drawLine(0, 0, width_, height_);
        painter->drawLine(0, height_, width_, 0);
        break;
    case App::Destroyed:
        painter->setBrush(Qt::red);        
        painter->drawRect(0, 0, width_, height_);
        break;
    case App::Hit:
        painter->setBrush(Qt::yellow);
        painter->drawRect(0, 0, width_, height_);
        break;
    case App::Life:
    case App::NearbyShip:
    case App::Empty:
        if (disable_ && status_ == App::Life) {
            painter->setBrush(Qt::blue);
            painter->drawRect(0, 0, width_, height_);
            return;
        }
        painter->setBrush(Qt::cyan);
        painter->drawRect(0, 0, width_, height_);

        if (hover_) {
            painter->drawLine(0, 0, width_, height_);
            painter->drawLine(0, height_, width_, 0);
        }
        break;
    default:
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

//----------------------------------------------------------------------------------------------------------------------

TextCell::TextCell(double x, double y, int w, int h, const QString &text)
    : width_(w), height_(h), text_(text)
{
    setPos(x, y);
}

QRectF TextCell::boundingRect() const
{
    return QRectF(0, 0, width_, height_);
}

void TextCell::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->setFont(QFont("Arial", 20));
    painter->drawText(boundingRect(), Qt::AlignCenter, text_);
}

//----------------------------------------------------------------------------------------------------------------------

TurnIndicator::TurnIndicator(double x, double y, int w, int h)
    : width_(w), height_(h), color_(Qt::green), dir_(App::Right)
{
    setPos(x, y);
    points_ = {
        {0, height_ / 3.0},
        {0, height_ * 2 / 3.0},
        {width_ * 2 / 3.0, height_ * 2 / 3.0},
        {width_ * 2 / 3.0, height_ / 1.0},
        {width_ / 1.0 , height_ / 2.0},
        {width_ * 2 / 3.0, 0.0},
        {width_ * 2 / 3.0, height_ / 3.0},
    };
}

QRectF TurnIndicator::boundingRect() const
{
    return QRectF(0, 0, width_, height_);
}

void TurnIndicator::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->setPen(Qt::black);
    painter->setBrush(color_);

    switch (dir_) {
    case App::Right:
        painter->rotate(0);
        break;
    case App::Left:
        painter->translate(width_, height_);
        painter->rotate(180);
        break;
    default:
        break;
    }
    painter->drawPolygon(points_.constData(), 7);
}

void TurnIndicator::change(App::Direction dir, Qt::GlobalColor color)
{
    dir_ = dir;
    color_ = color;
    update();
}

void TurnIndicator::setDiraction(App::Direction dir)
{
    dir_ = dir;
}

void TurnIndicator::setColor(Qt::GlobalColor color)
{
    color_ = color;
}
