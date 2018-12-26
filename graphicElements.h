#ifndef GRAHICELEMENTS_H
#define GRAHICELEMENTS_H

#include <QtWidgets>
#include "settings.h"
#include "ship.h"

class MenuButton : public QGraphicsObject
{
    Q_OBJECT

public:
    MenuButton(const QString &title, e_ButtonType type = e_ButtonType::Standart);
    void setSize(double width, double heigth);
    void reset();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

signals:
    void clicked(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;    
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    QString m_title;
    double m_width;
    double m_heigth;
    bool m_hover;
    e_ButtonType m_type;
};

//----------------------------------------------------------------------------------------------------------------------

class Cell : public QGraphicsObject
{
    Q_OBJECT

public:
    Cell(int width, int heigth, int idX, int idY, bool disable = false);
    e_Status shot();
    e_Status status() const;
    void reset();
    bool isEmpty() const;
    void setStatus(e_Status status);
    void setShip(Ship *ship);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

signals:
    void clicked(int x, int y);

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int m_width;
    int m_height;
    int m_idX;
    int m_idY;
    bool m_hover;
    bool m_disable;
    e_Status m_status;
    Ship *m_pShip;
};

//----------------------------------------------------------------------------------------------------------------------

class TextCell : public QGraphicsItem
{
public:
    TextCell(double x, double y, int w, int h, const QString &text);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    int m_width;
    int m_height;
    QString m_text;
};

//----------------------------------------------------------------------------------------------------------------------

class GameMap : public QGraphicsObject
{
    Q_OBJECT
public:
    GameMap(int width, int height, bool disable = false);
    QVector<QVector<Cell *>> &mesh();
    void resetStatusMesh();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

signals:
    void clicked(int x, int y);

private:
    int m_width;
    int m_height;
    QVector<QVector<Cell *>> m_mesh;
};

//----------------------------------------------------------------------------------------------------------------------

class TurnIndicator : public QGraphicsObject
{
    Q_OBJECT

public:
    TurnIndicator(double x, double y, int w, int h);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void change(e_Direction dir, Qt::GlobalColor color);
    void setDiraction(e_Direction dir);
    void setColor(Qt::GlobalColor color);

private:
    QVector<QPointF> m_points;
    int m_width;
    int m_height;
    Qt::GlobalColor m_color;
    e_Direction m_dir;
};

#endif // GRAHICELEMENTS_H