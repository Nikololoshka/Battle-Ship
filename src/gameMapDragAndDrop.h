#ifndef GAMEMAPDRAGANDDROP_H
#define GAMEMAPDRAGANDDROP_H
/*
#include <QtWidgets>
#include "appNamespace.h"

class CellDragAndDrop;

class GameMapDragAndDrop : public QGraphicsObject
{
public:
    GameMapDragAndDrop(int width, int height);
    void setCellShip(int x, int y, Ship *ship);
    void setCellStatus(int x, int y, e_Status st);
    e_Status cellStatus(int x, int y) const;
    bool isEmptyCell(int x, int y) const;
    void resetStatusMesh();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    int m_width;
    int m_height;
    QVector<QVector<CellDragAndDrop *>> m_mesh;
};

//----------------------------------------------------------------------------------------------------------------------

class CellDragAndDrop
{
public:
    CellDragAndDrop(int width, int heigth, int idX, int idY, bool showShip = true);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void startDrag();

private:
    QPointF m_dragPos;
};
*/
#endif // GAMEMAPDRAGANDDROP_H
