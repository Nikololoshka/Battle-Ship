#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QtWidgets>
#include "utilities.h"

class MenuButton : public QGraphicsObject
{
    Q_OBJECT
public:
    MenuButton(const QString &title);
    MenuButton(const QString &title, double width, double height);
    void setSize(double width, double height);
    void setText(const QString &title);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

signals:
    void clicked(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    QString m_title;
    double m_width;
    double m_height;
    bool m_hover;
    QFont m_font;
};

#endif // MENUBUTTON_H
