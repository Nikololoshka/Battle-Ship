#include "textLabel.h"

TextLabel::TextLabel(const QString &text)
    : m_width(0), m_height(0), m_text(text)
{
}

TextLabel::TextLabel(double x, double y, double w, double h, const QString &text)
    : m_width(w), m_height(h), m_text(text)
{
    setPos(x, y);
}

void TextLabel::setSize(double width, double height)
{
    m_width = width;
    m_height = height;
}

QRectF TextLabel::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void TextLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    QFont correctFont = QApplication::font();
    correctFont.setPixelSize(static_cast<int>(m_height * 0.75));
    painter->setFont(correctFont);
    painter->drawText(boundingRect(), Qt::AlignCenter, m_text);
}
