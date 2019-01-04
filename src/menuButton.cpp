#include "menuButton.h"

MenuButton::MenuButton(const QString &title)
    : QGraphicsObject (),
      m_title(title),
      m_width(0),
      m_height(0),
      m_hover(false)
{
    setAcceptHoverEvents(true);
}

MenuButton::MenuButton(const QString &title, double width, double height)
    : QGraphicsObject (),
      m_title(title),
      m_width(width),
      m_height(height),
      m_hover(false)
{
    setAcceptHoverEvents(true);
}

void MenuButton::setSize(double width, double height)
{
    m_width = width;
    m_height = height;
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
    QFont correctFont = QApplication::font();
    correctFont.setPixelSize(static_cast<int>(m_height * 0.4));
    painter->setFont(correctFont);
    painter->drawRect(this->boundingRect());
    painter->drawText(this->boundingRect(), Qt::AlignCenter | Qt::TextWordWrap, m_title);
}

void MenuButton::mousePressEvent(QGraphicsSceneMouseEvent */*event*/)
{
}

void MenuButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(this);
    m_hover = false;
    QGraphicsItem::mouseReleaseEvent(event);
}


void MenuButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = true;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void MenuButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = false;
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}
