#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtWidgets>
#include "ship.h"

class Settings
{
public:
    static Settings &inst();

    int animationDelay();
    void setAnimationDelay(int animationDelay);

    QSize windowSize() const;
    QRect sceneRect() const;
    void setWindowSize(const QSize &windowSize);

    QString playerName() const;
    void setPlayerName(const QString &playerName);

    QString language() const;
    void setLanguage(const QString &language);

    QSharedPointer<QTranslator> translator() const;
    void setTranslator(QTranslator *translator);

private:
    Settings();
    ~Settings();
    Settings(const Settings &other) = delete;
    Settings& operator=(const Settings &other) = delete;
    void load();
    void save();

private:
    QSharedPointer<QTranslator> m_pTranslator;
    QSize m_windowSize;
    QString m_playerName;
    QString m_language;
    int m_animationDelay;
};

class SettingsChangeEvent : public QEvent
{
public:
    SettingsChangeEvent();
    static QEvent::Type typeEvent();
};

#endif // SETTINGS_H
