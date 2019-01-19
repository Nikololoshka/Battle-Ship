#ifndef BOT_H
#define BOT_H

#include <QtWidgets>
#include "utilities.h"
#include "player.h"

class Bot : public Player
{
public:
    enum e_Difficulty {
        Easy,
        Medium
    };

    Bot(QString name, QObject *parent = nullptr);
    e_Status turn(Player *otherPlayer);
    void setDifficulty(e_Difficulty difficulty);
    int botX() const;
    int botY() const;

private:
    e_Status easyDifficulty(Player *otherPlayer);
    e_Status mediumDifficulty(Player *otherPlayer);

private:
    bool m_expertMode;
    bool m_changeShotDirection;
    int m_botX;
    int m_botY;
    int m_primaryX;
    int m_primaryY;
    e_Direction m_shotDirection;
    e_Difficulty m_difficulty;
};

#endif // BOT_H
