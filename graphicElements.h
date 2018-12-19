#ifndef GRAHICELEMENTS_H
#define GRAHICELEMENTS_H

#include <QtWidgets>
#include "settings.h"
#include "battleShipNamespace.h"
#include "ship.h"

class MenuButton : public QGraphicsObject
{
    Q_OBJECT

public:
    MenuButton(const QString &title, double width, double heigth, App::ButtonType type = App::Standart);
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
    App::ButtonType type_;
};

//----------------------------------------------------------------------------------------------------------------------

class Cell : public QGraphicsObject
{
    Q_OBJECT

public:
    Cell(double x, double y, int w, int h, int id, bool disable = false);
    App::Status shot(int x, int y);
    App::Status status() const;
    void setStatus(App::Status stat);
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
    App::Status status_;
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
    TurnIndicator(double x, double y, int w, int h);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void change(App::Direction dir, Qt::GlobalColor color);
    void setDiraction(App::Direction dir);
    void setColor(Qt::GlobalColor color);

private:
    QVector<QPointF> points_;
    int width_;
    int height_;
    Qt::GlobalColor color_;
    App::Direction dir_;
};

#endif // GRAHICELEMENTS_H
