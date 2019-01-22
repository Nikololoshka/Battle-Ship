#include "gameMapEditor.h"

GameMapEditor::GameMapEditor(int width, int height, const QString &textLayout)
    : QGraphicsObject (),
      m_RIGHT_BORDER(g_MAP_SIZE + 5),
      m_width(width),
      m_height(height),
      m_isDropAllowed(false)
{
    setAcceptDrops(true);

    for (int i = 0; i < g_MAP_SIZE; ++i)
        m_mesh.append(QVector<QSharedPointer<CellDragAndDrop>>(m_RIGHT_BORDER));

    m_textLayout.resize(g_MAP_SIZE);

    const int dx = m_width / (m_RIGHT_BORDER + 2);
    const int dy = m_height / (g_MAP_SIZE + 1);

    for (int i = 0; i < g_MAP_SIZE + 1; ++i) {
        for (int j = 0; j < g_MAP_SIZE + 1; ++j) {
            if (i == 0 && j == 0) {
                continue;
            } else if (i == 0) {
                auto temp = new TextLabel(dx * j, dy * i, dx, dy, textLayout.at(j - 1));
                temp->setParentItem(this);
                m_textLayout[j - 1].reset(temp);
            } else if (j == 0) {
                TextLabel *temp = new TextLabel(dx * j, dy * i, dx, dy, QString::number(i));
                temp->setParentItem(this);
            } else {
                auto temp = new CellDragAndDrop(dx, dy, i - 1, j - 1);
                temp->setPos(dx * j, dy * i);
                temp->setParent(this);
                temp->setParentItem(this);
                m_mesh[i - 1][j - 1].reset(temp);
            }
        }
    }

    m_pClearPlaceMode = new QGraphicsRectItem(this);
    m_pClearPlaceMode->setVisible(false);

    for (int i = 1; i < g_MAP_SIZE + 1; ++i) {
        for (int j = g_MAP_SIZE + 2; j < m_RIGHT_BORDER + 2; ++j) {
            auto temp = new CellDragAndDrop(dx, dy, i - 1, j - 2);
            temp->setPos(dx * j, dy * i);
            temp->setParent(this);
            temp->setParentItem(m_pClearPlaceMode);
            m_mesh[i - 1][j - 2].reset(temp);
        }
    }
}

void GameMapEditor::setLabelText(const QString &textLayout)
{
    for (int i = 0; i < m_textLayout.size(); ++i) {
        m_textLayout[i]->setText(textLayout.at(i));
    }
}


void GameMapEditor::moveShipsOnGameMap(QSharedPointer<GameMap> &map, QVector<QSharedPointer<Ship>> &ships)
{
    for (int i = 0; i < g_MAP_SIZE; ++i) {
        for (int j = 0; j < g_MAP_SIZE; ++j) {
            map->setCellShip(i, j, m_mesh[i][j]->ship());
            map->setCellStatus(i, j, m_mesh[i][j]->status());
        }
    }
    ships.swap(m_ships);
    reset();
}

bool GameMapEditor::isReady()
{
    if (!m_pClearPlaceMode->isVisible())
        return true;

    for (int i = 0; i < g_MAP_SIZE; ++i) {
        for (int j = g_MAP_SIZE; j < m_RIGHT_BORDER; ++j) {
            if (m_mesh[i][j]->status() == e_Status::Life)
                return false;
        }
    }
    return true;
}

void GameMapEditor::setClearMapMode()
{
    reset();
    m_pClearPlaceMode->setVisible(true);
    setShipsOnClearMap();
}

void GameMapEditor::setGenerateRandomMode()
{
    reset();
    m_pClearPlaceMode->setVisible(false);
    setRandomShips();
}

void GameMapEditor::reset()
{
    for (int i = 0; i < g_MAP_SIZE; ++i)
        for (int j = 0; j < m_RIGHT_BORDER; ++j)
            m_mesh[i][j]->reset();

    for (auto &ship : m_ships)
        ship->reset();
}

QVector<QSharedPointer<Ship>> GameMapEditor::ships() const
{
    return m_ships;
}

void GameMapEditor::setShips(const QVector<QSharedPointer<Ship>> &ships)
{
    m_ships = ships;
}

QRectF GameMapEditor::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void GameMapEditor::paint(QPainter */*painter*/, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
}

void GameMapEditor::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat(MimeDataOfShip::mimeType())) {
        event->acceptProposedAction();
    }
}

void GameMapEditor::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    // coord change places because x and y are different for scene and mesh
    int x = static_cast<int>(event->pos().y() * (g_MAP_SIZE + 1) / m_height) - 1;
    int y = static_cast<int>(event->pos().x() * (m_RIGHT_BORDER + 2) / m_width) - 1;

    clearDropEffect(x, y);
    m_isDropAllowed = false;

    if (x >= 0 && y >= 0) {
        if (y == g_MAP_SIZE)
            return;

        if (y >= g_MAP_SIZE) {
            if (!m_pClearPlaceMode->isVisible()) {
                return;
            }
            --y;
        }

        auto data = dynamic_cast<const MimeDataOfShip *>(event->mimeData());

        int index = -1;
        if ((index = data->ship->numberCell(data->dragX, data->dragY)) == -1) {
            return;
        }

        int dx = x - data->ship->body().at(index).x;
        int dy = y - data->ship->body().at(index).y;

        // check for drop inside the map and empty cells
        bool isLeftMap = false, isRightMap = false;
        for (const auto &el : data->ship->body()) {
            if (el.y + dy < g_MAP_SIZE)
                isLeftMap = true;

            if (el.y + dy >= g_MAP_SIZE)
                isRightMap = true;

            if (isLeftMap && isRightMap) {
                return;
            }
        }
        for (const auto &el : data->ship->body()) {
            if (!isCorrectPlaceForShip(el.x + dx, el.y + dy, data->ship, isLeftMap)) {
                return;
            }
        }

        // display "drop effect" where to put the ship
        for (const auto &el : data->ship->body()) {
            m_mesh[el.x + dx][el.y + dy]->setDropEffect(true);
            m_mesh[el.x + dx][el.y + dy]->update();
        }
        m_isDropAllowed = true;

    }
}

void GameMapEditor::dropEvent(QGraphicsSceneDragDropEvent *event)
{    
    if (m_isDropAllowed) {
        // coord change places because x and y are different for scene and mesh
        int x = static_cast<int>(event->pos().y() * (g_MAP_SIZE + 1) / m_height) - 1;
        int y = static_cast<int>(event->pos().x() * (m_RIGHT_BORDER + 2) / m_width) - 1;

        if (y >= g_MAP_SIZE)
            --y;

        auto data = dynamic_cast<const MimeDataOfShip *>(event->mimeData());
        clearDropEffect(x, y);

        int index = data->ship->numberCell(data->dragX, data->dragY);

        int dx = x - data->ship->body().at(index).x;
        int dy = y - data->ship->body().at(index).y;

        for (auto &el : data->ship->body()) {   // remove ship from old coords
            removeNearbyShipStatus(el.x, el.y, data->ship);
            m_mesh[el.x][el.y]->reset();
        }
        index = 0;
        for (auto &el : data->ship->body()) {   // set ship to the new coords
            m_mesh[el.x + dx][el.y + dy]->setStatus(e_Status::Life);
            m_mesh[el.x + dx][el.y + dy]->setShip(data->ship);
            setNearbyShipStatus(el.x + dx, el.y + dy);
            data->ship->setCellCoord(index, el.x + dx, el.y + dy);
            ++index;
        }
        update();
    }
}

void GameMapEditor::setRandomShips()
{
    int x = 0, y = 0, dir = 0;

    for (int i = 0; i < 10; i++) {
        do {
            x = generateRandomNumber(0, g_MAP_SIZE - 1);
            y = generateRandomNumber(0, g_MAP_SIZE - 1);
            dir = generateRandomNumber(0, 1);
        } while (!setShip(m_ships[i], x, y, dir));
    }
}

bool GameMapEditor::setShip(QSharedPointer<Ship> &ship, int x, int y, bool isHor)
{
    bool correct = true;

    if (isHor ? x - ship->length() < 0 : y + ship->length() >= g_MAP_SIZE) {
        return false;
    } else {
        for (int i = 0, j = 0; i < ship->length() && j < ship->length(); isHor ? i++ : j++) {
            if (m_mesh[x - i][y + j]->status() != e_Status::Empty) {
                correct = false;
                break;
            }
        }
    }
    if (correct) {
        ship->setOrientation(!isHor ? Ship::Horizontal : Ship::Vertical);
        for (int a = 0, b = 0; a < ship->length() && b < ship->length(); isHor ? a++ : b++) {
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    if (i == 0 && j == 0) {
                        m_mesh[x - a][y + b]->setStatus(e_Status::Life);
                        m_mesh[x - a][y + b]->setShip(ship);
                        ship->setCellCoord(a + b, x - a, y + b);
                    } else if (x + i - a < g_MAP_SIZE && x + i - a >= 0 &&
                             y + j + b < g_MAP_SIZE && y + j + b >= 0 &&
                             m_mesh[x + i - a][y + j + b]->status() != e_Status::Life) {
                        m_mesh[x + i - a][y + j + b]->setStatus(e_Status::NearbyShip);
                    }
                }
            }
        }
        return true;
    }
    return false;
}

void GameMapEditor::setShipsOnClearMap()
{
    int x = -1;
    int y = g_MAP_SIZE;

    for (int i = 0; i < m_ships.size(); ++i) {
        if (x + m_ships[i]->length() < g_MAP_SIZE) {
            x += 1;
        } else {
            x = 0;
            y += 2;
        }

        m_ships[i]->setOrientation(Ship::Vertical);
        for (int j = 0; j < m_ships[i]->length(); ++j) {
            m_ships[i]->setCellCoord(m_ships[i]->length() - j - 1, x, y);
            m_mesh[x][y]->setStatus(e_Status::Life);
            m_mesh[x][y]->setShip(m_ships[i]);
            setNearbyShipStatus(x, y);
            ++x;
        }
    }
}

void GameMapEditor::setNearbyShipStatus(int x, int y)
{
    for (int i = -1; i < 2; ++i)
        for (int j = -1; j < 2; ++j)
            if (i != 0 || j != 0)
                if (isCorrectCoords(x + i, y + j, isLeftMap(y)))
                    if (m_mesh[x + i][y + j]->status() == e_Status::Empty)
                        m_mesh[x + i][y + j]->setStatus(e_Status::NearbyShip);
}

void GameMapEditor::removeNearbyShipStatus(int x, int y, const QSharedPointer<Ship> &ship)
{
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            if (i != 0 || j != 0) {
                if (isCorrectCoords(x + i, y + j, isLeftMap(y))) {
                    bool correct = true;
                    for (int q = -1; q < 2; ++q) {
                        for (int p = -1; p < 2; ++p)  {
                            if (isCorrectCoords(x + i + q, y + j + p, isLeftMap(y))) {
                                if (m_mesh[x + i + q][y + j + p]->status() == e_Status::Life &&
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
                        m_mesh[x + i][y + j]->setStatus(e_Status::Empty);
                }
            }
        }
    }
}

bool GameMapEditor::isCorrectPlaceForShip(int x, int y, const QSharedPointer<Ship> &ship, bool isLeftPartMap) const
{
    if (isCorrectCoords(x, y, isLeftPartMap)) {
        if (m_mesh[x][y]->status() == e_Status::Life  && ship->numberCell(x, y) == -1) {
            return false;
        }
        if (m_mesh[x][y]->status() == e_Status::NearbyShip) {
            for (int i = -1; i < 2; ++i) {
                for (int j = -1; j < 2; ++j) {
                    if (isCorrectCoords(x + i, y + j, isLeftPartMap)) {
                        if (m_mesh[x + i][y + j]->status() == e_Status::Life &&
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

bool GameMapEditor::isCorrectCoords(int x, int y, bool isLeftPartMap) const
{
    if (x < g_MAP_SIZE && x >= 0)
        if (y < (isLeftPartMap ? g_MAP_SIZE : m_RIGHT_BORDER) &&
                y >= (isLeftPartMap ? 0 : g_MAP_SIZE))
            return true;

    return false;
}

bool GameMapEditor::isLeftMap(int y)
{
    return y < g_MAP_SIZE;
}

void GameMapEditor::clearDropEffect(int x, int y)
{
    for (int i = x - 5; i < x + 5; ++i) {
        for (int j = y - 5; j < y + 5; ++j) {
            if (i >= 0 && j >= 0 && i < g_MAP_SIZE && j < m_RIGHT_BORDER) {
                m_mesh[i][j]->setDropEffect(false);
                m_mesh[i][j]->update();
            }
        }
    }
}

void GameMapEditor::rotateShip(int x, int y, QSharedPointer<Ship> ship)
{
    int rotatePos = ship->numberCell(x, y);
    int rotateInvert = ship->orientation() == Ship::Vertical ? -1 : 1;

    bool isLeftMap = true;
    if (y >= g_MAP_SIZE) {
        --y;
        isLeftMap = false;
    }

    for (auto &el : ship->body()) {
        int rotateValue = ship->numberCell(el.x, el.y) - rotatePos;
        if (!isCorrectPlaceForShip(el.x - rotateValue * rotateInvert,
                                   el.y - rotateValue * rotateInvert,
                                   ship, isLeftMap)) {
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
        m_mesh[el.x][el.y]->setStatus(e_Status::Life);
        m_mesh[el.x][el.y]->setShip(ship);
        setNearbyShipStatus(el.x, el.y);
    }

    ship->setOrientation(ship->orientation() == Ship::Horizontal ? Ship::Vertical : Ship::Horizontal);
    update();
}

//----------------------------------------------------------------------------------------------------------------------

GameMapEditor::MimeDataOfShip::MimeDataOfShip()
    : QMimeData ()
{
}

QString GameMapEditor::MimeDataOfShip::mimeType()
{
    return "app/mimeDataOfShip";
}

//----------------------------------------------------------------------------------------------------------------------

GameMapEditor::CellDragAndDrop::CellDragAndDrop(int width, int heigth, int idX, int idY)
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

void GameMapEditor::CellDragAndDrop::reset()
{
    m_status = e_Status::Empty;
    m_pShip.reset();
}

bool GameMapEditor::CellDragAndDrop::dropEffect() const
{
    return m_dropEffect;
}

void GameMapEditor::CellDragAndDrop::setDropEffect(bool dropEffect)
{
    m_dropEffect = dropEffect;
}

QSharedPointer<Ship> &GameMapEditor::CellDragAndDrop::ship()
{
    return m_pShip;
}

void GameMapEditor::CellDragAndDrop::setShip(const QSharedPointer<Ship> &ship)
{
    m_pShip = ship;
}

e_Status GameMapEditor::CellDragAndDrop::status() const
{
    return m_status;
}

void GameMapEditor::CellDragAndDrop::setStatus(const e_Status &status)
{
    m_status = status;
}

QRectF GameMapEditor::CellDragAndDrop::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void GameMapEditor::CellDragAndDrop::paint(QPainter *painter,
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

void GameMapEditor::CellDragAndDrop::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_status == e_Status::Life && event->button() == Qt::LeftButton)
        m_dragPos = event->pos();

//    QGraphicsObject::mousePressEvent(event);
}

void GameMapEditor::CellDragAndDrop::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_status == e_Status::Life && (event->buttons() & Qt::LeftButton)) {
        auto distance = (event->pos() - m_dragPos).manhattanLength();
        if (distance > QApplication::startDragDistance()) {
            this->startDrag();
        }
    }
    QGraphicsObject::mouseMoveEvent(event);
}

void GameMapEditor::CellDragAndDrop::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_pShip->length() > 1 && m_status == e_Status::Life) {
        dynamic_cast<GameMapEditor *>(this->parent())->rotateShip(m_idX, m_idY, m_pShip);
    }

    QGraphicsObject::mouseDoubleClickEvent(event);
}

void GameMapEditor::CellDragAndDrop::startDrag()
{
    MimeDataOfShip *mimeData = new MimeDataOfShip;
    mimeData->ship = m_pShip;
    mimeData->dragX = m_idX;
    mimeData->dragY = m_idY;

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);


    QPixmap dragIcon(":/image/ship" + QString::number(m_pShip->length()));
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

