#include "menuSelectedButton.h"

MenuSelectedButton::MenuSelectedButton()
    : QGraphicsObject (),
      m_width(0),
      m_height(0),
      m_hover(false),
      m_correctValue(0),
      m_prefix()
{
    setAcceptHoverEvents(true);
}

MenuSelectedButton::MenuSelectedButton(double width, double height)
    : QGraphicsObject (),
      m_width(width),
      m_height(height),
      m_hover(false),
      m_correctValue(0)
{
    setAcceptHoverEvents(true);
}

void MenuSelectedButton::setSize(double width, double height)
{
    m_width = width;
    m_height = height;
}

void MenuSelectedButton::addOption(const QString &name, Bot::e_Difficulty value)
{
    m_options.append({name, value});
}

void MenuSelectedButton::updateTranslate(const QString &name, Bot::e_Difficulty value)
{
    for (auto &el : m_options) {
        if (el.second == value) {
            el.first = name;
            break;
        }
    }
}

void MenuSelectedButton::setPrefix(const QString &prefix)
{
    m_prefix = prefix;
}

QRectF MenuSelectedButton::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void MenuSelectedButton::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    if (m_hover)
        painter->setBrush(Qt::blue);
    else
        painter->setBrush(Qt::cyan);

    painter->setPen(Qt::black);
    QFont correctFont = QApplication::font();
    correctFont.setPixelSize(static_cast<int>(m_height * 0.35));
    painter->setFont(correctFont);
    painter->drawRect(this->boundingRect());

    auto dx = m_width / (m_options.size() + 1);

    for (int i = 0; i < m_options.size(); ++i) {
        painter->setBrush(Qt::darkYellow);
        if (i == m_correctValue) {
            painter->setFont(computeFontForText(QRectF(0, 0, m_width, m_height * 0.6).toRect(),
                                                Qt::AlignCenter | Qt::TextWordWrap,
                                                m_prefix + ": " + m_options[i].first));
            painter->drawText(QRectF(0, 0, m_width, m_height * 0.6),
                              Qt::AlignCenter | Qt::TextWordWrap,
                              m_prefix + ": " + m_options[i].first);
            painter->setBrush(Qt::magenta);
        }
        painter->drawEllipse(QPointF((i + 1) * dx, m_height * 0.8), m_height * 0.1, m_height * 0.1);
    }
}

void MenuSelectedButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = true;
    update();
    QGraphicsObject::hoverEnterEvent(event);
}

void MenuSelectedButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = false;
    update();
    QGraphicsObject::hoverLeaveEvent(event);
}

void MenuSelectedButton::mousePressEvent(QGraphicsSceneMouseEvent */*event*/)
{
//    QGraphicsObject::mousePressEvent(event);
}

void MenuSelectedButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_correctValue = (m_correctValue + 1) % m_options.size();
    update();
    emit changeValue(m_options[m_correctValue].second);
    QGraphicsObject::mouseReleaseEvent(event);
}

