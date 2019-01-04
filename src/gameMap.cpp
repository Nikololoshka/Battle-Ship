#include "gameMap.h"

GameMap::GameMap(int width, int height, bool disable)
    : QGraphicsObject (), m_width(width), m_height(height)
{
    for (int i = 0; i < g_MAP_SIZE; ++i)
        m_mesh.append(QVector<Cell *>(g_MAP_SIZE, nullptr));

    const int dx = m_width / (g_MAP_SIZE + 1);
    const int dy = m_height / (g_MAP_SIZE + 1);

    for (int i = 0; i < g_MAP_SIZE + 1; ++i) {
        for (int j = 0; j < g_MAP_SIZE + 1; ++j) {
            if (i == 0 && j == 0) {
                continue;
            } else if (i == 0) {
                TextLabel *temp = new TextLabel(dx * j, dy * i, dx, dy, QChar('A' + j - 1));
                temp->setParentItem(this);
            } else if (j == 0) {
                TextLabel *temp = new TextLabel(dx * j, dy * i, dx, dy, QString::number(i));
                temp->setParentItem(this);
            } else {
                auto temp = new Cell(dx, dy, i - 1, j - 1, disable);
                temp->setPos(dx * j, dy * i);
                temp->setParentItem(this);

                if (!disable) {
                    temp->setAcceptHoverEvents(true);
                    connect(temp, &Cell::clicked, this, &GameMap::clicked);
                }
                m_mesh[i - 1][j - 1] = temp;                
            }
        }
    }
    this->setVisible(false);
}

e_Status GameMap::shot(int x, int y)
{
    switch (m_mesh[x][y]->m_status) {
    case e_Status::Empty:                           // если пусто
    case e_Status::NearbyShip:                      // то промах
        this->setCellStatus(x, y, e_Status::Miss);
        return e_Status::Miss;
    case e_Status::Life:
        switch(m_mesh[x][y]->m_pShip->shot(x, y)) {
        case e_Status::Hit:                         // если попали
            this->setCellStatus(x, y, e_Status::Hit);
            return  e_Status::Hit;
        case e_Status::Destroyed:                   // если уничтожили
            this->setCellStatus(x, y, e_Status::Destroyed);
            return  e_Status::Destroyed;
        default:                                    // если не попали
            return e_Status::Impossible;            // что, в принципе, не возможно
        }
    default:                                        // если повторно стреляем
        return e_Status::Impossible;                // туда же
    }
}

void GameMap::setCellShip(int x, int y, Ship *ship)
{
    m_mesh[x][y]->m_pShip = ship;
}

void GameMap::setCellStatus(int x, int y, e_Status st)
{
    m_mesh[x][y]->m_status = st;
    m_mesh[x][y]->update();
}

e_Status GameMap::cellStatus(int x, int y) const
{
    return m_mesh[x][y]->m_status;
}

bool GameMap::isEmptyCell(int x, int y) const
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

void GameMap::resetStatusMesh()
{
    for (int i = 0; i < g_MAP_SIZE; ++i)
        for (int j = 0; j < g_MAP_SIZE; ++j)
            m_mesh[i][j]->reset();
}

QRectF GameMap::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void GameMap::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->setPen(Qt::NoPen);
    painter->drawRect(this->boundingRect());
}

//----------------------------------------------------------------------------------------------------------------------

Cell::Cell(int width, int heigth, int idX, int idY, bool showShip)
    : QGraphicsObject (),
      m_status(e_Status::Empty),
      m_width(width),
      m_height(heigth),
      m_idX(idX),
      m_idY(idY),
      m_hover(false),
      m_showShip(showShip)
{
}

void Cell::reset()
{
    m_status = e_Status::Empty;
    m_pShip = nullptr;
}

QRectF Cell::boundingRect() const
{
    return  QRectF(0, 0, m_width, m_height);
}

void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    switch (m_status) {
    case e_Status::Miss:
        painter->setBrush(Qt::cyan);
        painter->drawRect(0, 0, m_width, m_height);
        painter->setPen(QPen(Qt::red, 2));
        painter->drawLine(0, 0, m_width, m_height);
        painter->drawLine(0, m_height, m_width, 0);
        break;
    case e_Status::Destroyed:
        painter->setBrush(Qt::red);
        painter->drawRect(0, 0, m_width, m_height);
        break;
    case e_Status::Hit:
        painter->setBrush(Qt::yellow);
        painter->drawRect(0, 0, m_width, m_height);
        break;
    case e_Status::Life:
    case e_Status::NearbyShip:
    case e_Status::Empty:
        if (m_showShip && m_status == e_Status::Life) {
            painter->setBrush(Qt::blue);
            painter->drawRect(0, 0, m_width, m_height);
            return;
        }
        painter->setBrush(Qt::cyan);
        painter->drawRect(0, 0, m_width, m_height);

        if (m_hover) {
            painter->drawLine(0, 0, m_width, m_height);
            painter->drawLine(0, m_height, m_width, 0);
        }
        break;
    default:
        break;
    }
}

void Cell::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = true;
    update();
    QGraphicsItem::hoverMoveEvent(event);
}

void Cell::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = false;
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}

void Cell::mousePressEvent(QGraphicsSceneMouseEvent */*event*/)
{
}

void Cell::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(m_idX, m_idY);
    QGraphicsItem::mouseReleaseEvent(event);
}

