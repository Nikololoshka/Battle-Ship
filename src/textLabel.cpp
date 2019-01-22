#include "textLabel.h"

TextLabel::TextLabel(const QString &text)
    : m_width(0),
      m_height(0),
      m_text(text)
{
    m_font = QApplication::font();
}

TextLabel::TextLabel(double x, double y, double w, double h, const QString &text)
    : m_width(w),
      m_height(h),
      m_text(text)
{
    setPos(x, y);
    m_font = computeFontForText(boundingRect().toRect(), Qt::AlignCenter, m_text, 0.95);
}

void TextLabel::setSize(double width, double height)
{
    m_width = width;
    m_height = height;
    m_font = computeFontForText(boundingRect().toRect(), Qt::AlignCenter, m_text, 0.95);
}

void TextLabel::setText(const QString &text)
{
    m_text = text;
    m_font = computeFontForText(boundingRect().toRect(), Qt::AlignCenter, m_text, 0.95);
    update();
}

QRectF TextLabel::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void TextLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->setFont(m_font);
    painter->drawText(boundingRect(), Qt::AlignCenter, m_text);
}
