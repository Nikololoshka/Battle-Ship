#ifndef MENUDISABLEBUTTON_H
#define MENUDISABLEBUTTON_H

#include <QtWidgets>

class MenuDisableButton : public QGraphicsItem
{
public:
    MenuDisableButton(const QString &title);
    MenuDisableButton(const QString &title, double width, double heigth);
    void setSize(double width, double heigth);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QString m_title;
    double m_width;
    double m_height;
};

#endif // MENUDISABLEBUTTON_H
