#ifndef MENUDISABLEBUTTON_H
#define MENUDISABLEBUTTON_H

#include <QtWidgets>
#include "utilities.h"

class MenuDisableButton : public QGraphicsItem
{
public:
    MenuDisableButton(const QString &title);
    MenuDisableButton(const QString &title, double width, double heigth);
    void setSize(double width, double heigth);    
    void setText(const QString &title);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QString m_title;
    double m_width;
    double m_height;
    QFont m_font;
};

#endif // MENUDISABLEBUTTON_H
