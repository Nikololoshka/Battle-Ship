#include "gameMapDragAndDrop.h"
/*
GameMapDragAndDrop::GameMapDragAndDrop(int width, int height)
    : QGraphicsObject (),
      m_width(width),
      m_height(height)
{
    for (int i = 0; i < MAP_SIZE; ++i)
        m_mesh.append(QVector<CellDragAndDrop *>(MAP_SIZE, nullptr));

    const int dx = m_width / (MAP_SIZE + 1);
    const int dy = m_height / (MAP_SIZE + 1);

    for (int i = 0; i < MAP_SIZE + 1; ++i) {
        for (int j = 0; j < MAP_SIZE + 1; ++j) {
            if (i == 0 && j == 0) {
                continue;
            } else if (i == 0) {
                TextLabel *temp = new TextLabel(dx * j, dy * i, dx, dy, QChar('A' + j - 1));
                temp->setParentItem(this);
            } else if (j == 0) {
                TextLabel *temp = new TextLabel(dx * j, dy * i, dx, dy, QString::number(i));
                temp->setParentItem(this);
            } else {
                auto temp = new CellDragAndDrop(dx, dy, i - 1, j - 1);
                temp->setPos(dx * j, dy * i);
                temp->setParentItem(this);
                m_mesh[i - 1][j - 1] = temp;
            }
        }
    }
}

void GameMapDragAndDrop::setCellShip(int x, int y, Ship *ship)
{
    m_mesh[x][y]->m_pShip = ship;
}

void GameMapDragAndDrop::setCellStatus(int x, int y, e_Status st)
{
    m_mesh[x][y]->m_status = st;
    m_mesh[x][y]->update();
}

e_Status GameMapDragAndDrop::cellStatus(int x, int y) const
{
    return m_mesh[x][y]->m_status;
}

bool GameMapDragAndDrop::isEmptyCell(int x, int y) const
{
    switch (m_mesh[x][y]->m_status) {
    case e_Status::Miss:
    case e_Status::Hit:
    case e_Status::Destroyed:
        return false;
    default:
        return true;
    }
}

void GameMapDragAndDrop::resetStatusMesh()
{
    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j)
            m_mesh[i][j]->reset();
}

QRectF GameMapDragAndDrop::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}
*/
//void GameMapDragAndDrop::paint(QPainter */*painter*/, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
//{
//}

//----------------------------------------------------------------------------------------------------------------------
/*
CellDragAndDrop::CellDragAndDrop(int width, int heigth, int idX, int idY, bool showShip)
    : Cell(width, heigth, idX, idY, showShip)
{
}

void CellDragAndDrop::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_dragPos = event->pos();

    QGraphicsItem::mousePressEvent(event);
}

void CellDragAndDrop::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        auto distance = (event->pos() - m_dragPos).manhattanLength();
        if (distance > QApplication::startDragDistance()) {
            this->startDrag();
        }
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void CellDragAndDrop::startDrag()
{
    QMimeData *mimeData = new QMimeData;
    mimeData->setText(QString::number(m_idX) +
                      " : " +
                      QString::number(m_idY));

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(QApplication::style()->standardPixmap(QStyle::SP_DirOpenIcon));
    drag->exec();
}
*/
