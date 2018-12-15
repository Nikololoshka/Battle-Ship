#ifndef GRAHICELEMENTS_H
#define GRAHICELEMENTS_H

#include <QtWidgets>

class MenuButton : public QGraphicsObject
{
    Q_OBJECT

public:
    MenuButton(const QString &title, double x, double y, double width, double heigth);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void clicked();

private:
    QString title_;
    double x_;
    double y_;
    double width_;
    double heigth_;
};


#endif // GRAHICELEMENTS_H
