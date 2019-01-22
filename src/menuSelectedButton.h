#ifndef MENUSELECTEDBUTTON_H
#define MENUSELECTEDBUTTON_H

#include <QtWidgets>
#include "bot.h"

class MenuSelectedButton : public QGraphicsObject
{
    Q_OBJECT
public:
    MenuSelectedButton();
    MenuSelectedButton(double width, double height);

    void setSize(double width, double height);
    void addOption(const QString &name, Bot::e_Difficulty value);
    void updateTranslate(const QString &name, Bot::e_Difficulty value);
    void setPrefix(const QString &prefix);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

signals:
    void changeValue(Bot::e_Difficulty value);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    double m_width;
    double m_height;
    bool m_hover;
    int m_correctValue;
    QString m_prefix;
    QVector<QPair<QString, Bot::e_Difficulty>> m_options;
};

#endif // MENUSELECTEDBUTTON_H
