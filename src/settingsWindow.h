#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QtWidgets>
#include "settings.h"

class SettingsWindow : public QDialog
{
    Q_OBJECT
public:
    SettingsWindow(QWidget *parent = nullptr);

protected:
    void changeEvent(QEvent *event) override;

private slots:
    void clickedButtonOk();
    void clickedButtonApply();
    void clickedButtonCancel();

private:
    QGroupBox *m_pGroupGeneralSetting;
    QLabel *m_pLocaleListLabel;
    QComboBox *m_pLocaleListComboBox;
    QLabel *m_pPlayerNameLabel;
    QLineEdit *m_pPlayerNameLineEdit;

    QGroupBox *m_pGroupGameSetting;
    QLabel *m_pAnimationSpeedLabel;
    QSlider *m_pAnimationSpeedSlider;
    QSpinBox *m_pAnimationSpeedSpinBox;

    QPushButton *m_pButtonOk;
    QPushButton *m_pButtonApply;
    QPushButton *m_pButtonCancel;
};

#endif // SETTINGSWINDOW_H
