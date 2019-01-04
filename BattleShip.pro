#-------------------------------------------------
#
# Project created by QtCreator 2018-12-14T16:58:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BattleShip
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += C++17

HEADERS += \
    src/appNamespace.h \
    src/battleShipCore.h \
    src/battleShipView.h \
    src/battleShipWindow.h \
    src/bot.h \
    src/gameMap.h \
    src/gameMapDragAndDrop.h \
    src/menuButton.h \
    src/player.h \
    src/settings.h \
    src/ship.h \
    src/textLabel.h \
    src/turnIndicator.h \
    src/menuSelectedButton.h \
    src/menuDisableButton.h

SOURCES += \
    src/battleShipCore.cpp \
    src/battleShipView.cpp \
    src/battleShipWindow.cpp \
    src/bot.cpp \
    src/gameMap.cpp \
    src/gameMapDragAndDrop.cpp \
    src/main.cpp \
    src/menuButton.cpp \
    src/player.cpp \
    src/settings.cpp \
    src/ship.cpp \
    src/textLabel.cpp \
    src/turnIndicator.cpp \
    src/menuDisableButton.cpp \
    src/menuSelectedButton.cpp

RESOURCES += \
    res/res.qrc
