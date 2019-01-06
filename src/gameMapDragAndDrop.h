#ifndef GAMEMAPDRAGANDDROP_H
#define GAMEMAPDRAGANDDROP_H

#include <QtWidgets>
#include "settings.h"
#include "gameMap.h"
#include "textLabel.h"
#include "utilities.h"

class GameMapDragAndDrop : public QGraphicsObject
{
public:
    GameMapDragAndDrop(int width, int height);
    ~GameMapDragAndDrop() override;

    void setShipsOnGameMap(GameMap *map, QVector<Ship *> &ships);
    void reset();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

private:
    void setShips();    
    void setNearbyShipStatus(int x, int y);
    void removeNearbyShipStatus(int x, int y);
    void clearDropEffect();
    void rotateShip(int x, int y, Ship *ship);

private:
    struct MimeDataOfShip : public QMimeData
    {
        MimeDataOfShip();
        static QString mimeType();

        Ship *ship;
        int dragX;
        int dragY;
    };

    class CellDragAndDrop : public QGraphicsObject
    {
    public:
        CellDragAndDrop(int width, int heigth, int idX, int idY);

        void reset();

        QRectF boundingRect() const override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        e_Status m_status;
        Ship *m_pShip;
        bool m_dropEffect;
    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    private:
        void startDrag();

    private:
        int m_width;
        int m_height;
        int m_idX;
        int m_idY;
        QPointF m_dragPos;
    };

    int m_width;
    int m_height;
    bool m_isDropAllowed;
    QVector<QVector<CellDragAndDrop *>> m_mesh;
    QVector<Ship *> m_ships;
};

#endif // GAMEMAPDRAGANDDROP_H
