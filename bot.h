#ifndef BOT_H
#define BOT_H

#include <QtWidgets>
#include "player.h"
#include "settings.h"

class Bot : public Player
{
public:
    Bot(QString name, QObject *parent = nullptr);
    e_Status turn(Player *otherPlayer);
    int getBotX() const;
    int getBotY() const;

private:
    bool m_expertMode;
    bool m_changeShotDirection;
    int m_botX;
    int m_botY;
    int m_primaryX;
    int m_primaryY;
    e_Direction m_shotDirection;
};

#endif // BOT_H
