#include "menuDisableButton.h"

MenuDisableButton::MenuDisableButton(const QString &title)
    : QGraphicsItem (),
      m_title(title),
      m_width(0),
      m_height(0)
{
    m_font = QApplication::font();
}

MenuDisableButton::MenuDisableButton(const QString &title, double width, double heigth)
    : QGraphicsItem (),
      m_title(title),
      m_width(width),
      m_height(heigth)
{
    m_font = computeFontForText(boundingRect().toRect(), Qt::AlignCenter | Qt::TextWordWrap, m_title);
}

void MenuDisableButton::setSize(double width, double heigth)
{
    m_width = width;
    m_height = heigth;
    m_font = computeFontForText(boundingRect().toRect(), Qt::AlignCenter | Qt::TextWordWrap, m_title);
}

void MenuDisableButton::setText(const QString &title)
{
    m_title = title;
    m_font = computeFontForText(boundingRect().toRect(), Qt::AlignCenter | Qt::TextWordWrap, m_title);
}

QRectF MenuDisableButton::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void MenuDisableButton::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    QLinearGradient gradient(0, 0, m_width, m_height);
    for (int i = 0; i < 50; ++i) {
        if (i % 2 == 0)
            gradient.setColorAt(i / 50.0, Qt::gray);
        else
            gradient.setColorAt(i / 50.0, Qt::white);
    }
    painter->setBrush(gradient);

    painter->setPen(Qt::black);
    painter->setFont(m_font);
    painter->drawRect(this->boundingRect());
    painter->drawText(this->boundingRect(), Qt::AlignCenter | Qt::TextWordWrap, m_title);
}
