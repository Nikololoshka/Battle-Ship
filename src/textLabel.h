#ifndef TEXTLABEL_H
#define TEXTLABEL_H

#include <QtWidgets>
#include "utilities.h"

class TextLabel : public QGraphicsItem
{
public:
    TextLabel(const QString &text);
    TextLabel(double x, double y, double w, double h, const QString &text);
    void setSize(double width, double height);
    void setText(const QString &text);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    double m_width;
    double m_height;
    QString m_text;
    QFont m_font;
};

#endif // TEXTLABEL_H
