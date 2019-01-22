#include "gameMap.h"

GameMap::GameMap(int width, int height, bool disable, const QString &textLayout)
    : QGraphicsObject (), m_width(width), m_height(height)
{
    for (int i = 0; i < g_MAP_SIZE; ++i)
        m_mesh.push_back(QVector<QSharedPointer<Cell>>(g_MAP_SIZE));

    m_textLayout.resize(g_MAP_SIZE);

    const int dx = m_width / (g_MAP_SIZE + 1);
    const int dy = m_height / (g_MAP_SIZE + 1);

    for (int i = 0; i < g_MAP_SIZE + 1; ++i) {
        for (int j = 0; j < g_MAP_SIZE + 1; ++j) {
            if (i == 0 && j == 0) {
                continue;
            } else if (i == 0) {
                auto temp = new TextLabel(dx * j, dy * i, dx, dy,  textLayout.at(j - 1));
                temp->setParentItem(this);
                m_textLayout[j - 1].reset(temp);
            } else if (j == 0) {
                TextLabel *temp = new TextLabel(dx * j, dy * i, dx, dy, QString::number(i));
                temp->setParentItem(this);
            } else {
                auto temp = new Cell(dx, dy, i - 1, j - 1);
                temp->setPos(dx * j, dy * i);
                temp->setParentItem(this);

                if (!disable) {
                    temp->setAcceptHoverEvents(true);
                    temp->setShowShip(false);
                }
                m_mesh[i - 1][j - 1].reset(temp);
            }
        }
    }
}


e_Status GameMap::shot(int x, int y)
{
    switch (m_mesh[x][y]->status()) {
    case e_Status::Empty:                           // если пусто
    case e_Status::NearbyShip:                      // то промах
        this->setCellStatus(x, y, e_Status::Miss);
        return e_Status::Miss;
    case e_Status::Life:
        switch(m_mesh[x][y]->ship()->shot(x, y)) {
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

void GameMap::setCellShip(int x, int y, QSharedPointer<Ship> &ship)
{
    m_mesh[x][y]->setShip(ship);
}

void GameMap::setCellStatus(int x, int y, e_Status st)
{
    m_mesh[x][y]->setStatus(st);
    m_mesh[x][y]->update();
}

e_Status GameMap::cellStatus(int x, int y) const
{
    return m_mesh[x][y]->status();
}

bool GameMap::isEmptyCell(int x, int y) const
{
    switch (m_mesh[x][y]->status()) {
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

void GameMap::setDestroyedArea(int x, int y)
{
    e_Direction dir = e_Direction::None;
    int tempX = x, tempY = y;
    bool find = false;

    while (true) {
        setDestroyedAreaImpl(x, y, dir);
        find = false;
        while (!find) {
            x = tempX;
            y = tempY;
            dir = static_cast<e_Direction>((static_cast<int>(dir) + 1) % 5);
            switch (dir) {
            case e_Direction::Right:
                x++;
                break;
            case e_Direction::Left:
                x--;
                break;
            case e_Direction::Up:
                y--;
                break;
            case e_Direction::Down:
                y++;
                break;
            default:
                return;
            }
            if (x >= g_MAP_SIZE || y >= g_MAP_SIZE || x < 0 || y < 0)
                continue;

            if (cellStatus(x, y) == e_Status::Destroyed)
                find = true;
        }
    }
}

void GameMap::setLabelText(const QString &textLayout)
{
    for (int i = 0; i < m_textLayout.size(); ++i) {
        m_textLayout[i]->setText(textLayout.at(i));
    }
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

void GameMap::setDestroyedAreaImpl(int x, int y, e_Direction dir)
{
    bool find;
    do {
        find = false;

        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                if ((i + x) < g_MAP_SIZE && (i + x) >= 0 &&
                    (j + y) < g_MAP_SIZE && (j + y) >= 0) {
                    if (cellStatus(x + i, y + j) == e_Status::NearbyShip)
                        setCellStatus(x + i, y + j, e_Status::Miss);
                    if (cellStatus(x + i, y + j) == e_Status::Hit)
                        setCellStatus(x + i, y + j, e_Status::Destroyed);
                }
            }
        }
        switch (dir) {
        case e_Direction::Right:
            x++;
            break;
        case e_Direction::Left:
            x--;
            break;
        case e_Direction::Up:
            y--;
            break;
        case e_Direction::Down:
            y++;
            break;
        default:
            return;
        }
        if (x >= g_MAP_SIZE || y >= g_MAP_SIZE || x < 0 || y < 0)
            continue;

        if (cellStatus(x, y) == e_Status::Destroyed)
            find = true;

    } while (find);
}

//----------------------------------------------------------------------------------------------------------------------

GameMap::Cell::Cell(int width, int heigth, int idX, int idY)
    : QGraphicsObject (),
      m_status(e_Status::Empty),
      m_width(width),
      m_height(heigth),
      m_idX(idX),
      m_idY(idY),
      m_hover(false),
      m_showShip(true)
{
}

void GameMap::Cell::setShowShip(bool flag)
{
    m_showShip = flag;
}

void GameMap::Cell::reset()
{
    m_status = e_Status::Empty;
    m_pShip.reset();
}

QSharedPointer<Ship> &GameMap::Cell::ship()
{
    return m_pShip;
}

void GameMap::Cell::setShip(const QSharedPointer<Ship> &ship)
{
    m_pShip = ship;
}

e_Status GameMap::Cell::status() const
{
    return m_status;
}

void GameMap::Cell::setStatus(const e_Status &status)
{
    m_status = status;
}

QRectF GameMap::Cell::boundingRect() const
{
    return  QRectF(0, 0, m_width, m_height);
}

void GameMap::Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
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

void GameMap::Cell::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = true;
    update();
    QGraphicsObject::hoverMoveEvent(event);
}

void GameMap::Cell::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = false;
    update();
    QGraphicsObject::hoverLeaveEvent(event);
}

void GameMap::Cell::mousePressEvent(QGraphicsSceneMouseEvent */*event*/)
{
//    QGraphicsObject::mousePressEvent(event);
}

void GameMap::Cell::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (!m_showShip)
        emit qobject_cast<GameMap *>(this->parentObject())->clicked(m_idX, m_idY);

    QGraphicsObject::mouseReleaseEvent(event);
}


