#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <QtWidgets>
#include "textLabel.h"
#include "ship.h"

class GameMap : public QGraphicsObject
{
    Q_OBJECT
public:
    GameMap(int width, int height, bool disable = false, const QString &textLayout = "ABCDEFGHIJ");
    e_Status shot(int x, int y);

    void setCellShip(int x, int y, QSharedPointer<Ship> &ship);
    void setCellStatus(int x, int y, e_Status st);
    e_Status cellStatus(int x, int y) const;
    bool isEmptyCell(int x, int y) const;    
    void resetStatusMesh();
    void setDestroyedArea(int x, int y);
    void setLabelText(const QString &textLayout);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

signals:
    void clicked(int x, int y);

private:
    void setDestroyedAreaImpl(int x, int y, e_Direction dir);

private:
    class Cell : public QGraphicsObject
    {
    public:
        Cell(int width, int heigth, int idX, int idY);
        void setShowShip(bool flag);
        void reset();

        QRectF boundingRect() const override;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        e_Status status() const;
        void setStatus(const e_Status &status);
        QSharedPointer<Ship> &ship();
        void setShip(const QSharedPointer<Ship> &ship);

    private:
        void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    private:
        e_Status m_status;
        QSharedPointer<Ship> m_pShip;
        int m_width;
        int m_height;
        int m_idX;
        int m_idY;
        bool m_hover;
        bool m_showShip;
    };

    int m_width;
    int m_height;
    QVector<QSharedPointer<TextLabel>> m_textLayout;
    QVector<QVector<QSharedPointer<Cell>>> m_mesh;
};

#endif // GAMEMAP_H
