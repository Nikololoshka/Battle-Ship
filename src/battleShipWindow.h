#ifndef BATTLESHIPWINDOW_H
#define BATTLESHIPWINDOW_H

#include <QtWidgets>
#include "settingsWindow.h"
#include "settings.h"
#include "battleShipView.h"

class BattleShipWindow : public QMainWindow
{
    Q_OBJECT
public:
    BattleShipWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent *event) override;
    bool event(QEvent *event) override;

private slots:
    void aboutBattleShip();
    void settingWindow();

private:
    QMenu *m_pMenuGame;
    QAction *m_pSettingsMenuGame;
    QAction *m_pAboutMenuGame;
    QAction *m_pExitMenuGame;

    BattleShipView *m_pView;
};

#endif // BATTLESHIPWINDOW_H
