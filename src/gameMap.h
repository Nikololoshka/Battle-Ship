#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <QtWidgets>
#include "settings.h"
#include "textLabel.h"
#include "ship.h"

class Cell;

class GameMap : public QGraphicsObject
{
    Q_OBJECT
public:
    GameMap(int width, int height, bool disable = false);
    e_Status shot(int x, int y);

    void setCellShip(int x, int y, Ship *ship);
    void setCellStatus(int x, int y, e_Status st);

    e_Status cellStatus(int x, int y) const;
    bool isEmptyCell(int x, int y) const;
    void resetStatusMesh();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
signals:
    void clicked(int x, int y);

protected:
    int m_width;
    int m_height;
    QVector<QVector<Cell *>> m_mesh;
};

//----------------------------------------------------------------------------------------------------------------------

class Cell : public QGraphicsObject
{
    Q_OBJECT
public:
    Cell(int width, int heigth, int idX, int idY, bool showShip = true);
    void reset();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    e_Status m_status;
    Ship *m_pShip;

signals:
    void clicked(int x, int y);

private:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    int m_width;
    int m_height;
    int m_idX;
    int m_idY;
    bool m_hover;
    bool m_showShip;
};

#endif // GAMEMAP_H
