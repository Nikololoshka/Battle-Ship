#include "graphicElements.h"

MenuButton::MenuButton(const QString &title, e_ButtonType type)
    : QGraphicsObject (),
      m_title(title), m_width(0), m_height(0), m_hover(false), m_type(type)
{
    setAcceptHoverEvents(true);

    switch (m_type) {
    case e_ButtonType::Disable:
        setEnabled(false);
        break;
    default:
        break;
    }
}

void MenuButton::setSize(double width, double heigth)
{
    m_width = width;
    m_height = heigth;
}

void MenuButton::reset()
{
    m_hover = false;
}


QRectF MenuButton::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void MenuButton::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    switch (m_type) {
    case e_ButtonType::Disable:
    {
        QLinearGradient gradient(0, 0, m_width, m_height);
        for (int i = 0; i < 50; ++i) {
            if (i % 2 == 0)
                gradient.setColorAt(i / 50.0, Qt::gray);
            else
                gradient.setColorAt(i / 50.0, Qt::white);
        }
        painter->setBrush(gradient);
        break;
    }
    default:
        if (m_hover)
            painter->setBrush(Qt::blue);
        else
            painter->setBrush(Qt::cyan);
        break;
    }

    painter->setPen(Qt::black);
    QFont correctFont = QApplication::font();
    correctFont.setPixelSize(static_cast<int>(m_height * 0.4));
    painter->setFont(correctFont);
    painter->drawRect(this->boundingRect());
    painter->drawText(this->boundingRect(), Qt::AlignCenter | Qt::TextWordWrap, m_title);
}

void MenuButton::mousePressEvent(QGraphicsSceneMouseEvent */*event*/)
{
}

void MenuButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(this);
    m_hover = false;
    QGraphicsItem::mouseReleaseEvent(event);
}


void MenuButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = true;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void MenuButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = false;
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}

//----------------------------------------------------------------------------------------------------------------------

MenuSelectedButton::MenuSelectedButton()
    : QGraphicsObject (),
      m_width(0),
      m_height(0),
      m_hover(false),
      m_correctDifficulty(0)
{
    setAcceptHoverEvents(true);
}

void MenuSelectedButton::setSize(double width, double heigth)
{
    m_width = width;
    m_height = heigth;
}

void MenuSelectedButton::addOption(const QString &name, e_Difficulty difficulty)
{
    m_options.append({name, difficulty});
}

QRectF MenuSelectedButton::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void MenuSelectedButton::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    if (m_hover)
        painter->setBrush(Qt::blue);
    else
        painter->setBrush(Qt::cyan);

    painter->setPen(Qt::black);
    QFont correctFont = QApplication::font();
    correctFont.setPixelSize(static_cast<int>(m_height * 0.35));
    painter->setFont(correctFont);
    painter->drawRect(this->boundingRect());

    auto dx = m_width / (m_options.size() + 1);

    for (int i = 0; i < m_options.size(); ++i) {
        painter->setBrush(Qt::darkYellow);
        if (i == m_correctDifficulty) {
            painter->drawText(QRectF(0, 0, m_width, m_height * 0.6),
                              Qt::AlignCenter | Qt::TextWordWrap,
                              m_options[i].first);
            painter->setBrush(Qt::magenta);
        }
        painter->drawEllipse(QPointF((i + 1) * dx, m_height * 0.8), m_height * 0.1, m_height * 0.1);
    }
}

void MenuSelectedButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = true;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void MenuSelectedButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = false;
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}

void MenuSelectedButton::mousePressEvent(QGraphicsSceneMouseEvent */*event*/)
{
}

void MenuSelectedButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_correctDifficulty = (m_correctDifficulty + 1) % m_options.size();
    update();
    emit changeDifficulty(m_options[m_correctDifficulty].second);
    QGraphicsItem::mouseReleaseEvent(event);
}

//----------------------------------------------------------------------------------------------------------------------

Cell::Cell(int width, int heigth, int idX, int idY, bool disable)
    : QGraphicsObject (), m_width(width), m_height(heigth),
      m_idX(idX), m_idY(idY), m_hover(false), m_disable(disable),
      m_status(e_Status::Empty), m_pShip(nullptr)
{
    if (!disable)
        setAcceptHoverEvents(true);
    else
        setAcceptedMouseButtons(Qt::MouseButton::NoButton);
}

e_Status Cell::shot()
{
    switch (m_status) {
    case e_Status::Empty:         // если пусто
    case e_Status::NearbyShip:    // то промах
        m_status = e_Status::Miss;
        update();
        return e_Status::Miss;
    case e_Status::Life:
        switch(m_pShip->shot(m_idX, m_idY)) {
        case e_Status::Hit:         // если попали
            m_status = e_Status::Hit;
            update();
            return  e_Status::Hit;
        case e_Status::Destroyed:   // если уничтожили
            m_status = e_Status::Destroyed;
            update();
            return  e_Status::Destroyed;
        default:                // если не попали
            return e_Status::Impossible;   // что, в принципе, не возможно
        }
    default:                    // если повторно стреляем
        return e_Status::Impossible; // туда же
    }
}

void Cell::setStatus(e_Status status)
{
    m_status = status;
    update();
}

void Cell::setShip(Ship *ship)
{
    m_pShip = ship;
}

e_Status Cell::status() const
{
    return m_status;
}

void Cell::reset()
{
    m_status = e_Status::Empty;
    m_pShip = nullptr;
}

bool Cell::isEmpty() const
{
    switch (m_status) {
    case e_Status::Miss:
    case e_Status::Hit:
    case e_Status::Destroyed:
        return false;
    default:
        return true;
    }
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
        if (m_disable && m_status == e_Status::Life) {
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

//----------------------------------------------------------------------------------------------------------------------

TextLabel::TextLabel(const QString &text)
    : m_width(0), m_height(0), m_text(text)
{
}

TextLabel::TextLabel(double x, double y, double w, double h, const QString &text)
    : m_width(w), m_height(h), m_text(text)
{
    setPos(x, y);
}

void TextLabel::setSize(double width, double height)
{
    m_width = width;
    m_height = height;
}

QRectF TextLabel::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void TextLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    QFont correctFont = QApplication::font();
    correctFont.setPixelSize(static_cast<int>(m_height * 0.75));
    painter->setFont(correctFont);
    painter->drawText(boundingRect(), Qt::AlignCenter, m_text);
}

//----------------------------------------------------------------------------------------------------------------------

GameMap::GameMap(int width, int height, bool disable)
    : QGraphicsObject (), m_width(width), m_height(height)
{
    for (int i = 0; i < MAP_SIZE; ++i)
        m_mesh.append(QVector<Cell *>(MAP_SIZE, nullptr));

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
                auto temp = new Cell(dx, dy, i - 1, j - 1, disable);
                temp->setPos(dx * j, dy * i);
                temp->setParentItem(this);
                m_mesh[i - 1][j - 1] = temp;
                connect(temp, &Cell::clicked, this, &GameMap::clicked);
            }
        }
    }
}

QVector<QVector<Cell *> > &GameMap::mesh()
{
    return m_mesh;
}

void GameMap::resetStatusMesh()
{
    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j)
            m_mesh[i][j]->reset();
}

QRectF GameMap::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void GameMap::paint(QPainter */*painter*/, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
}

//----------------------------------------------------------------------------------------------------------------------

TurnIndicator::TurnIndicator(double x, double y, int w, int h)
    : m_width(w), m_height(h), m_color(Qt::green), m_dir(e_Direction::Right)
{
    setPos(x, y);
    m_points = {
        {0, m_height / 3.0},
        {0, m_height * 2 / 3.0},
        {m_width * 2 / 3.0, m_height * 2 / 3.0},
        {m_width * 2 / 3.0, m_height / 1.0},
        {m_width / 1.0, m_height / 2.0},
        {m_width * 2 / 3.0, 0.0},
        {m_width * 2 / 3.0, m_height / 3.0},
    };
}

QRectF TurnIndicator::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void TurnIndicator::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->setPen(Qt::black);
    painter->setBrush(m_color);

    switch (m_dir) {
    case e_Direction::Right:
        painter->rotate(0);
        break;
    case e_Direction::Left:
        painter->translate(m_width, m_height);
        painter->rotate(180);
        break;
    default:
        break;
    }
    painter->drawPolygon(m_points.constData(), 7);
}

void TurnIndicator::change(e_Direction dir, Qt::GlobalColor color)
{
    m_dir = dir;
    m_color = color;
    update();
}

void TurnIndicator::setDiraction(e_Direction dir)
{
    m_dir = dir;
}

void TurnIndicator::setColor(Qt::GlobalColor color)
{
    m_color = color;
}
