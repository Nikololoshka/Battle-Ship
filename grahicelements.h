#ifndef GRAHICELEMENTS_H
#define GRAHICELEMENTS_H

#include <QtWidgets>
#include "settings.h"
#include "ship.h"

class MenuButton : public QGraphicsObject
{
    Q_OBJECT

public:
    enum ButtonType {
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
    enum Status {
        Empty,
        NearbyShip,
        Life,
        Hit,
        Destroyed,
        Miss,
    };

    Cell(double x, double y, int w, int h, int id, bool disable = false);
    Ship::Status shot(int x, int y);
    Status status() const;
    void setStatus(Status stat);
    void setShip(Ship *ship);
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
    bool disable_;
    Status status_;
    Ship *ship_;
};

//----------------------------------------------------------------------------------------------------------------------

class TextCell : public QGraphicsItem
{
public:
    TextCell(double x, double y, int w, int h, const QString &text);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    int width_;
    int height_;
    QString text_;
};

//----------------------------------------------------------------------------------------------------------------------

class TurnIndicator : public QGraphicsObject
{
    Q_OBJECT

public:
    enum Diraction {
        Up,
        Left,
        Right,
        Down
    };
    TurnIndicator(double x, double y, int w, int h);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void change(Diraction dir, Qt::GlobalColor color);
    void setDiraction(Diraction dir);
    void setColor(Qt::GlobalColor color);

private:
    QVector<QPointF> points_;
    int width_;
    int height_;
    Qt::GlobalColor color_;
    Diraction dir_;
};

#endif // GRAHICELEMENTS_H
