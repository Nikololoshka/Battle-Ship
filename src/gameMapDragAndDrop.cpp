#include "gameMapDragAndDrop.h"

GameMapDragAndDrop::GameMapDragAndDrop(int width, int height)
    : QGraphicsObject (),
      m_width(width),
      m_height(height),
      m_isDropAllowed(false)
{
    for (int i = 0; i < g_MAP_SIZE; ++i)
        m_mesh.append(QVector<CellDragAndDrop *>(g_MAP_SIZE, nullptr));

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
                auto temp = new CellDragAndDrop(dx, dy, i - 1, j - 1);
                temp->setPos(dx * j, dy * i);
                temp->setParentItem(this);
                m_mesh[i - 1][j - 1] = temp;
            }
        }
    }
    m_ships = Settings::inst().standartShips();
    setShips();
    setAcceptDrops(true);
}

GameMapDragAndDrop::~GameMapDragAndDrop()
{
    for (int i = 0; i < g_MAP_SIZE; ++i)
        for (int j = 0; j < g_MAP_SIZE; ++j)
            if (m_mesh[i][j] != nullptr)
                delete m_mesh[i][j];

    for (auto &ship : m_ships)
        if (ship != nullptr)
            delete ship;
}

void GameMapDragAndDrop::setShipsOnGameMap(GameMap *map, QVector<Ship *> &ships)
{
    for (int i = 0; i < g_MAP_SIZE; ++i) {
        for (int j = 0; j < g_MAP_SIZE; ++j) {
            map->setCellShip(i, j, m_mesh[i][j]->m_pShip);
            map->setCellStatus(i, j, m_mesh[i][j]->m_status);
        }
    }

    ships = m_ships;
    this->reset();
}

void GameMapDragAndDrop::reset()
{
    for (int i = 0; i < g_MAP_SIZE; ++i)
        for (int j = 0; j < g_MAP_SIZE; ++j)
            m_mesh[i][j]->reset();

    for (auto &ship : m_ships)
        ship->reset();

    this->setShips();
}

QRectF GameMapDragAndDrop::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void GameMapDragAndDrop::paint(QPainter */*painter*/, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
}

void GameMapDragAndDrop::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat(MimeDataOfShip::mimeType())) {
        event->acceptProposedAction();
    }
}

void GameMapDragAndDrop::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    // coord change places because x and y are different for scene and mesh
    int x = static_cast<int>(event->pos().y() * (g_MAP_SIZE + 1) / m_height) - 1;
    int y = static_cast<int>(event->pos().x() * (g_MAP_SIZE + 1) / m_width) - 1;

    this->clearDropEffect();

    if (x >= 0 && y >= 0) {
        auto data = dynamic_cast<const MimeDataOfShip *>(event->mimeData());

        int index = -1;
        if ((index = data->ship->numberCell(data->dragX, data->dragY)) == -1) {
            m_isDropAllowed = false;
            return;
        }

        int dx = x - data->ship->body().at(index).x;
        int dy = y - data->ship->body().at(index).y;

        // check for drop inside the map and empty cells
        for (const auto &el : data->ship->body()) {
            if (el.y + dy >= 0 && el.y + dy < g_MAP_SIZE && el.x + dx >= 0 && el.x + dx < g_MAP_SIZE) {
                if (m_mesh[el.x + dx][el.y + dy]->m_status == e_Status::Life  &&
                        data->ship->numberCell(el.x + dx, el.y + dy) == -1) {
                    m_isDropAllowed = false;
                    return;
                }
                if (m_mesh[el.x + dx][el.y + dy]->m_status == e_Status::NearbyShip) {
                    for (int i = -1; i < 2; ++i)
                        for (int j = -1; j < 2; ++j)
                            if (el.x + dx + i < g_MAP_SIZE && el.x + dx + i >= 0 &&
                                    el.y + dy + j < g_MAP_SIZE && el.y + dy + j >= 0)
                                if (m_mesh[el.x + dx + i][el.y + dy + j]->m_status == e_Status::Life &&
                                        data->ship->numberCell(el.x + dx + i, el.y + dy + j) == -1) {
                                    m_isDropAllowed = false;
                                    return;
                                }
                }
            } else {
                m_isDropAllowed = false;
                return;
            }
        }

        // display "drop effect" where to put the ship
        for (const auto &el : data->ship->body()) {
            if (el.y + dy >= 0 && el.y + dy < g_MAP_SIZE && el.x + dx >= 0 && el.x + dx < g_MAP_SIZE) {
                m_mesh[el.x + dx][el.y + dy]->m_dropEffect = true;
                m_mesh[el.x + dx][el.y + dy]->update();
            }
        }
        m_isDropAllowed = true;

    } else {
        m_isDropAllowed = false;
    }
}

void GameMapDragAndDrop::dropEvent(QGraphicsSceneDragDropEvent *event)
{    
    if (m_isDropAllowed) {
        // coord change places because x and y are different for scene and mesh
        int x = static_cast<int>(event->pos().y() * (g_MAP_SIZE + 1) / m_height) - 1;
        int y = static_cast<int>(event->pos().x() * (g_MAP_SIZE + 1) / m_width) - 1;

        auto data = dynamic_cast<const MimeDataOfShip *>(event->mimeData());
        this->clearDropEffect();

        int index = data->ship->numberCell(data->dragX, data->dragY);

        int dx = x - data->ship->body().at(index).x;
        int dy = y - data->ship->body().at(index).y;

        index = 0;
        QVector<QPair<int, int>> oldCoords;
        for (auto &el : data->ship->body()) {   // remove ship from old coords
            oldCoords.append({el.x, el.y});
            m_mesh[el.x][el.y]->m_status = e_Status::Empty;
        }
        for (auto &el : data->ship->body()) {   // set ship to the new coords
            m_mesh[el.x + dx][el.y + dy]->m_status = e_Status::Life;
            m_mesh[el.x + dx][el.y + dy]->m_pShip = data->ship;
            setNearbyShipStatus(el.x + dx, el.y + dy);
            data->ship->setCellCoord(index, el.x + dx, el.y + dy);
            ++index;
        }

        for (auto &coord : oldCoords)
            removeNearbyShipStatus(coord.first, coord.second);

//        qDebug() << "+++++++++++++++++++++++++++++++";
//        for (int i = 0; i < g_MAP_SIZE; ++i) {
//            QString temp = "";
//            for (int j = 0; j < g_MAP_SIZE; ++j) {
//                if (m_mesh[i][j]->m_status == e_Status::Life)
//                    temp.append("#");
//                else if (m_mesh[i][j]->m_status == e_Status::NearbyShip)
//                    temp.append("*");
//                else
//                    temp.append("-");
//            }
//            qDebug() << temp;
//        }
    }
}

void GameMapDragAndDrop::setShips()
{
    int x = 1;
    int y = 0;

    for (int i = 0; i < m_ships.size(); ++i) {
        if (i > 0) {
            if (m_ships[i]->length() == m_ships[i - 1]->length()) {
                y += 2;
            } else {
                x += 2;
                y = 0;
            }
        }
        for (int j = 0; j < m_ships[i]->length(); ++j) {
            m_ships[i]->setCellCoord(j, x, y);
            m_mesh[x][y]->m_status = e_Status::Life;
            m_mesh[x][y]->m_pShip = m_ships[i];
            setNearbyShipStatus(x, y);
            ++y;
        }
    }
}

void GameMapDragAndDrop::setNearbyShipStatus(int x, int y)
{
    for (int i = -1; i < 2; ++i)
        for (int j = -1; j < 2; ++j)
            if (i != 0 || j != 0)
                if (x + i < g_MAP_SIZE && x + i >= 0 && y + j < g_MAP_SIZE && y + j >= 0)
                    if (m_mesh[x + i][y + j]->m_status == e_Status::Empty)
                        m_mesh[x + i][y + j]->m_status = e_Status::NearbyShip;
}

void GameMapDragAndDrop::removeNearbyShipStatus(int x, int y)
{
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            if (i != 0 || j != 0) {
                if (x + i < g_MAP_SIZE && x + i >= 0 && y + j < g_MAP_SIZE && y + j >= 0) {
                    bool correct = true;
                    for (int q = -1; q < 2; ++q) {
                        for (int p = -1; p < 2; ++p)  {
                            if (x + i + q < g_MAP_SIZE && x + i + q >= 0 && y + j + p < g_MAP_SIZE && y + j + p >= 0) {
                                if (m_mesh[x + i + q][y + j + p]->m_status == e_Status::Life) {
                                    correct = false;
                                    break;
                                }
                            }
                        }
                        if (!correct)
                            break;
                    }
                    if (correct)
                        m_mesh[x + i][y + j]->m_status = e_Status::Empty;                    
                }
            }
        }
    }
}

void GameMapDragAndDrop::clearDropEffect()
{
    for (int i = 0; i < g_MAP_SIZE; ++i) {
        for (int j = 0; j < g_MAP_SIZE; ++j) {
            m_mesh[i][j]->m_dropEffect = false;
            m_mesh[i][j]->update();
        }
    }
}

void GameMapDragAndDrop::rotateShip(int x, int y, Ship *ship)
{
    qDebug() << "rotate: " << x << ", " << y << " " << ship;
}

//----------------------------------------------------------------------------------------------------------------------

GameMapDragAndDrop::MimeDataOfShip::MimeDataOfShip()
    : QMimeData ()
{
}

QString GameMapDragAndDrop::MimeDataOfShip::mimeType()
{
    return "app/mimeDataOfShip";
}

//----------------------------------------------------------------------------------------------------------------------

GameMapDragAndDrop::CellDragAndDrop::CellDragAndDrop(int width, int heigth, int idX, int idY)
    : QGraphicsObject(),
      m_status(e_Status::Empty),
      m_pShip(nullptr),
      m_dropEffect(false),
      m_width(width),
      m_height(heigth),
      m_idX(idX),
      m_idY(idY)
{
}

void GameMapDragAndDrop::CellDragAndDrop::reset()
{
    m_status = e_Status::Empty;
    m_pShip = nullptr;
}

QRectF GameMapDragAndDrop::CellDragAndDrop::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void GameMapDragAndDrop::CellDragAndDrop::paint(QPainter *painter,
                                                const QStyleOptionGraphicsItem */*option*/,
                                                QWidget */*widget*/)
{
    switch (m_status) {
    case e_Status::Life:
        painter->setBrush(Qt::blue);
        painter->drawRect(0, 0, m_width, m_height);
        break;
    default:
        if (m_dropEffect)
            painter->setBrush(Qt::green);
        else
            painter->setBrush(Qt::cyan);

        painter->drawRect(0, 0, m_width, m_height);
        break;
    }
}

void GameMapDragAndDrop::CellDragAndDrop::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_status == e_Status::Life && event->button() == Qt::LeftButton)
        m_dragPos = event->pos();

//    QGraphicsObject::mousePressEvent(event);
}

void GameMapDragAndDrop::CellDragAndDrop::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_status == e_Status::Life && (event->buttons() & Qt::LeftButton)) {
        auto distance = (event->pos() - m_dragPos).manhattanLength();
        if (distance > QApplication::startDragDistance()) {
            this->startDrag();
        }
    }
    QGraphicsObject::mouseMoveEvent(event);
}

void GameMapDragAndDrop::CellDragAndDrop::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_status == e_Status::Life)
        dynamic_cast<GameMapDragAndDrop *>(this->parentObject())->rotateShip(m_idX, m_idY, m_pShip);

    QGraphicsObject::mouseDoubleClickEvent(event);
}

void GameMapDragAndDrop::CellDragAndDrop::startDrag()
{
    MimeDataOfShip *mimeData = new MimeDataOfShip;
    mimeData->ship = m_pShip;
    mimeData->dragX = m_idX;
    mimeData->dragY = m_idY;

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(QApplication::style()->standardPixmap(QStyle::SP_DirOpenIcon));
    drag->exec();
}
