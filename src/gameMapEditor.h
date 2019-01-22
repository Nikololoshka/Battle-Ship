#ifndef GAMEMAPDEDITOR_H
#define GAMEMAPDEDITOR_H

#include <QtWidgets>
#include "gameMap.h"
#include "textLabel.h"
#include "utilities.h"

class GameMapEditor : public QGraphicsObject
{
public:
    GameMapEditor(int width, int height, const QString &textLayout = "ABCDEFGHIJ");
    void setLabelText(const QString &text);
    void moveShipsOnGameMap(QSharedPointer<GameMap> &playerMap, QVector<QSharedPointer<Ship>> &playerShips);
    bool isReady();
    void setClearMapMode();
    void setGenerateRandomMode();
    void reset();

    QVector<QSharedPointer<Ship>> ships() const;
    void setShips(const QVector<QSharedPointer<Ship>> &ships);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;    

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

private:
    void setShipsOnClearMap();
    void setRandomShips();
    bool setShip(QSharedPointer<Ship> &ship, int x, int y, bool isHor);

    void setNearbyShipStatus(int x, int y);
    void removeNearbyShipStatus(int x, int y, const QSharedPointer<Ship> &ship);
    bool isCorrectPlaceForShip(int x, int y, const QSharedPointer<Ship> &ship, bool isLeftPartMap) const;
    bool isCorrectCoords(int x, int y, bool isLeftPartMap) const;
    bool isLeftMap(int y);
    void clearDropEffect(int x, int y);
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

        e_Status status() const;
        void setStatus(const e_Status &status);

        QSharedPointer<Ship> &ship();
        void setShip(const QSharedPointer<Ship> &ship);

        bool dropEffect() const;
        void setDropEffect(bool dropEffect);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    private:
        void startDrag();

    private:
        e_Status m_status;
        QSharedPointer<Ship> m_pShip;
        bool m_dropEffect;
        int m_width;
        int m_height;
        int m_idX;
        int m_idY;
        QPointF m_dragPos;
    };

    const int m_RIGHT_BORDER;
    int m_width;
    int m_height;
    bool m_isDropAllowed;
    QGraphicsRectItem *m_pClearPlaceMode;
    QVector<QSharedPointer<TextLabel>> m_textLayout;
    QVector<QVector<QSharedPointer<CellDragAndDrop>>> m_mesh;
    QVector<QSharedPointer<Ship>> m_ships;
};

#endif // GAMEMAPDEDITOR_H
