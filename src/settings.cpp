#include "settings.h"

Settings &Settings::inst()
{
    static Settings single;
    return single;
}

int Settings::animationDelay()
{
    return m_animationDelay;
}

Settings::Settings()
{
    load();
}

Settings::~Settings()
{
    save();
}

void Settings::load()
{
    QSettings setting("config.ini", QSettings::IniFormat);
    m_windowSize = setting.value("Window-Size", QSize(800, 600)).toSize();
    m_playerName = setting.value("Player-Name", "player").toString();
    m_language = setting.value("Language", "en").toString();
    m_animationDelay = setting.value("Animation-Delay", 500).toInt();
}

void Settings::save()
{
    QSettings setting("config.ini", QSettings::IniFormat);
    setting.setValue("Window-Size", m_windowSize);
    setting.setValue("Player-Name", m_playerName);
    setting.setValue("Language", m_language);
    setting.setValue("Animation-Delay", m_animationDelay);
}

QSharedPointer<QTranslator> Settings::translator() const
{
    return m_pTranslator;
}

void Settings::setTranslator(QTranslator *translator)
{
    m_pTranslator.reset(translator);
}

QString Settings::language() const
{
    return m_language;
}

void Settings::setLanguage(const QString &language)
{
    m_language = language;
}

QString Settings::playerName() const
{
    return m_playerName;
}

void Settings::setPlayerName(const QString &playerName)
{
    m_playerName = playerName;
}

QSize Settings::windowSize() const
{
    return m_windowSize;
}

QRect Settings::sceneRect() const
{
    return QRect(0, 0, 800, 600);
}

void Settings::setWindowSize(const QSize &windowSize)
{
    m_windowSize = windowSize;
}

void Settings::setAnimationDelay(int animationDelay)
{
    m_animationDelay = animationDelay;
}

//----------------------------------------------------------------------------------------------------------------------

SettingsChangeEvent::SettingsChangeEvent()
    : QEvent(static_cast<QEvent::Type>(QEvent::User + 200))
{

}

QEvent::Type SettingsChangeEvent::typeEvent()
{
    return static_cast<QEvent::Type>(QEvent::User + 200);
}
