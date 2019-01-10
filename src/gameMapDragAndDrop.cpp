#include "gameMapDragAndDrop.h"

GameMapDragAndDrop::GameMapDragAndDrop(int width, int height)
    : QGraphicsObject (),
      m_width(width),
      m_height(height),
      m_isDropAllowed(false)
{
    for (int i = 0; i < g_MAP_SIZE; ++i)
        m_mesh.push_back(QVector<QSharedPointer<CellDragAndDrop>>(g_MAP_SIZE));

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
                m_mesh[i - 1][j - 1].reset(temp);
            }
        }
    }
    m_ships = Settings::inst().standartShips();
    setShips();
    setAcceptDrops(true);
}


void GameMapDragAndDrop::setShipsOnGameMap(GameMap *map, QVector<QSharedPointer<Ship>> &ships)
{
    for (int i = 0; i < g_MAP_SIZE; ++i) {
        for (int j = 0; j < g_MAP_SIZE; ++j) {
            map->setCellShip(i, j, m_mesh[i][j]->m_pShip);
            map->setCellStatus(i, j, m_mesh[i][j]->m_status);
        }
    }
    ships.swap(m_ships);
    this->reset();
}

void GameMapDragAndDrop::reset()
{
    for (int i = 0; i < g_MAP_SIZE; ++i)
        for (int j = 0; j < g_MAP_SIZE; ++j)
            m_mesh[i][j]->reset();

    for (auto &ship : m_ships)
        ship->reset();

    setShips();
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

    clearDropEffect();

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
            if (!isCorrectPlaceForShip(el.x + dx, el.y + dy, data->ship)) {
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
        clearDropEffect();

        int index = data->ship->numberCell(data->dragX, data->dragY);

        int dx = x - data->ship->body().at(index).x;
        int dy = y - data->ship->body().at(index).y;

        for (auto &el : data->ship->body()) {   // remove ship from old coords
            removeNearbyShipStatus(el.x, el.y, data->ship);
            m_mesh[el.x][el.y]->reset();
        }
        index = 0;
        for (auto &el : data->ship->body()) {   // set ship to the new coords
            m_mesh[el.x + dx][el.y + dy]->m_status = e_Status::Life;
            m_mesh[el.x + dx][el.y + dy]->m_pShip = data->ship;
            setNearbyShipStatus(el.x + dx, el.y + dy);
            data->ship->setCellCoord(index, el.x + dx, el.y + dy);
            ++index;
        }
        update();
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
        m_ships[i]->setOrientation(Ship::Horizontal);
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

void GameMapDragAndDrop::removeNearbyShipStatus(int x, int y, const QSharedPointer<Ship> &ship)
{
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            if (i != 0 || j != 0) {
                if (x + i < g_MAP_SIZE && x + i >= 0 && y + j < g_MAP_SIZE && y + j >= 0) {
                    bool correct = true;
                    for (int q = -1; q < 2; ++q) {
                        for (int p = -1; p < 2; ++p)  {
                            if (x + i + q < g_MAP_SIZE && x + i + q >= 0 &&
                                    y + j + p < g_MAP_SIZE && y + j + p >= 0) {
                                if (m_mesh[x + i + q][y + j + p]->m_status == e_Status::Life &&
                                        ship->numberCell(x + i + q, y + j + p) == -1) {
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

bool GameMapDragAndDrop::isCorrectPlaceForShip(int x, int y, const QSharedPointer<Ship> &ship) const
{
    if (x >= 0 && x < g_MAP_SIZE && y >= 0 && y < g_MAP_SIZE) {
        if (m_mesh[x][y]->m_status == e_Status::Life  && ship->numberCell(x, y) == -1) {
            return false;
        }
        if (m_mesh[x][y]->m_status == e_Status::NearbyShip) {
            for (int i = -1; i < 2; ++i) {
                for (int j = -1; j < 2; ++j) {
                    if (x + i < g_MAP_SIZE && x + i >= 0 && y + j < g_MAP_SIZE && y + j >= 0) {
                        if (m_mesh[x + i][y + j]->m_status == e_Status::Life &&
                                ship->numberCell(x + i, y + j) == -1) {
                            return false;
                        }
                    }
                }
            }
        }
    } else {
        return false;
    }
    return true;
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

void GameMapDragAndDrop::rotateShip(int x, int y, QSharedPointer<Ship> ship)
{

    int rotatePos = ship->numberCell(x, y);
    int rotateInvert = ship->orientation() == Ship::Vertical ? -1 : 1;

    for (auto &el : ship->body()) {
        int rotateValue = ship->numberCell(el.x, el.y) - rotatePos;
        if (!isCorrectPlaceForShip(el.x - rotateValue * rotateInvert,
                                   el.y - rotateValue * rotateInvert, ship)) {
            return;
        }
    }

    for (auto &el : ship->body()) { // remove ship from old coords
        removeNearbyShipStatus(el.x, el.y, ship);
        m_mesh[el.x][el.y]->reset();
    }
    for (auto &el : ship->body()) { // set ship to the new coords
        int rotateValue = ship->numberCell(el.x, el.y) - rotatePos;
        el.x -= rotateValue * rotateInvert;
        el.y -= rotateValue * rotateInvert;
        m_mesh[el.x][el.y]->m_status = e_Status::Life;
        m_mesh[el.x][el.y]->m_pShip = ship;
        setNearbyShipStatus(el.x, el.y);
    }

    ship->setOrientation(ship->orientation() == Ship::Horizontal ? Ship::Vertical : Ship::Horizontal);
    update();
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
    m_pShip.reset();
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
    if (m_pShip->length() > 1 && m_status == e_Status::Life)
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


    QPixmap dragIcon(":/ship" + QString::number(m_pShip->length()));
    dragIcon = dragIcon.scaled(m_width * m_pShip->length(), m_height);

    // set opacity
    QImage image(dragIcon.size(), QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    painter.setOpacity(0.5);
    painter.drawPixmap(0, 0, dragIcon);
    painter.end();
    dragIcon = QPixmap::fromImage(image);

    if (m_pShip->orientation() == Ship::Vertical) {
        QMatrix matrix;
        matrix.rotate(90);
        dragIcon = dragIcon.transformed(matrix);
    }

    drag->setPixmap(dragIcon);

    if (m_pShip->orientation() == Ship::Vertical)
        drag->setHotSpot(QPoint(static_cast<int>(m_width * 0.5),
                                static_cast<int>(m_height * (m_pShip->length() - m_pShip->numberCell(m_idX, m_idY) - 0.5))));
    else
        drag->setHotSpot(QPoint(static_cast<int>(m_width * (m_pShip->numberCell(m_idX, m_idY) + 0.5)),
                                static_cast<int>(m_height * 0.5)));

    drag->exec();
}
