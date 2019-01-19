#include "menuButton.h"

MenuButton::MenuButton(const QString &title)
    : QGraphicsObject (),
      m_title(title),
      m_width(0),
      m_height(0),
      m_hover(false)
{
    setAcceptHoverEvents(true);
    m_font = QApplication::font();
}

MenuButton::MenuButton(const QString &title, double width, double height)
    : QGraphicsObject (),
      m_title(title),
      m_width(width),
      m_height(height),
      m_hover(false)
{
    setAcceptHoverEvents(true);
    m_font = computeFontForText(boundingRect().toRect(), Qt::AlignCenter | Qt::TextWordWrap, m_title);
}

void MenuButton::setSize(double width, double height)
{
    m_width = width;
    m_height = height;
    m_font = computeFontForText(boundingRect().toRect(), Qt::AlignCenter | Qt::TextWordWrap, m_title);
}

void MenuButton::setText(const QString &title)
{
    m_title = title;
    m_font = computeFontForText(boundingRect().toRect(), Qt::AlignCenter | Qt::TextWordWrap, m_title);
}

QRectF MenuButton::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void MenuButton::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{

    if (m_hover)
        painter->setBrush(Qt::blue);
    else
        painter->setBrush(Qt::cyan);

    painter->setPen(Qt::black);
    painter->setFont(m_font);
    painter->drawRect(this->boundingRect());
    painter->drawText(this->boundingRect(), Qt::AlignCenter | Qt::TextWordWrap, m_title);
}

void MenuButton::mousePressEvent(QGraphicsSceneMouseEvent */*event*/)
{
//    QGraphicsObject::mousePressEvent(event);
}

void MenuButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(this);
    m_hover = false;
    QGraphicsObject::mouseReleaseEvent(event);
}


void MenuButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = true;
    update();
    QGraphicsObject::hoverEnterEvent(event);
}

void MenuButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = false;
    update();
    QGraphicsObject::hoverLeaveEvent(event);
}
