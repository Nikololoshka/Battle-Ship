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

    void setShipsOnGameMap(GameMap *map, QVector<QSharedPointer<Ship>> &ships);
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
    void removeNearbyShipStatus(int x, int y, const QSharedPointer<Ship> &ship);
    bool isCorrectPlaceForShip(int x, int y, const QSharedPointer<Ship> &ship) const;
    void clearDropEffect();
    void rotateShip(int x, int y, QSharedPointer<Ship> ship);

private:
    struct MimeDataOfShip : public QMimeData
    {
        MimeDataOfShip();
        static QString mimeType();

        QSharedPointer<Ship> ship;
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
        QSharedPointer<Ship> m_pShip;
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
    QVector<QVector<QSharedPointer<CellDragAndDrop>>> m_mesh;
    QVector<QSharedPointer<Ship>> m_ships;
};

#endif // GAMEMAPDRAGANDDROP_H
