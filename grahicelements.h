#ifndef GRAHICELEMENTS_H
#define GRAHICELEMENTS_H

#include <QtWidgets>
#include "ship.h"

class MenuButton : public QGraphicsObject
{
    Q_OBJECT

public:
    enum class ButtonType {
        Standart,
        Disable
    };

    MenuButton(const QString &title, double width, double heigth, ButtonType type = ButtonType::Standart);
    void reset();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

signals:
    void clicked();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    QString title_;
    double width_;
    double heigth_;
    bool hover_;
    ButtonType type_;
};

//----------------------------------------------------------------------------------------------------------------------

class Cell : public QGraphicsObject
{
    Q_OBJECT

public:
    enum class Status {
        Life,
        Hit,
        Destroyed,
        Miss,
        Disable
    };

    Cell(double x, double y, int w, int h, int id, Status status = Status::Life);
    void shot();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

signals:
    void clicked(int id);

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int width_;
    int height_;
    int id_;
    bool hover_;
    Status status_;
    //Ship *ship_;
};

#endif // GRAHICELEMENTS_H
