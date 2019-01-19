#include "settingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog (parent)
{
    // initialization
    m_pLocaleListLabel = new QLabel(tr("Locale:"));
    m_pLocaleList = new QComboBox;

    m_pPlayerNameLabel = new QLabel(tr("Player name:"));
    m_pPlayerName = new QLineEdit;

    m_pButtonOk = new QPushButton(tr("OK"));
    m_pButtonApply = new QPushButton(tr("Apply"));
    m_pButtonCancel =  new QPushButton(tr("Cancel"));

    // settings
    setMinimumSize(400, 300);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Settings"));

    m_pLocaleList->addItem("English", "en");
    m_pLocaleList->addItem("Русский", "ru");

    for (int i = 0; i < m_pLocaleList->count(); ++i) {
       if (m_pLocaleList->itemData(i).toString() == Settings::inst().language()) {
           m_pLocaleList->setCurrentIndex(i);
           break;
       }
    }

    m_pPlayerName->setText(Settings::inst().playerName());

    // layout setup
    QHBoxLayout *layoutLocale = new QHBoxLayout;
    layoutLocale->addWidget(m_pLocaleListLabel);
    layoutLocale->addWidget(m_pLocaleList);

    QHBoxLayout *layoutPlayerName = new QHBoxLayout;
    layoutPlayerName->addWidget(m_pPlayerNameLabel);
    layoutPlayerName->addWidget(m_pPlayerName);

    QVBoxLayout *layoutGeneralSetting = new QVBoxLayout;
    layoutGeneralSetting->addLayout(layoutLocale);
    layoutGeneralSetting->addLayout(layoutPlayerName);

    m_pGroupGeneralSetting = new QGroupBox(tr("General settings"));
    m_pGroupGeneralSetting->setLayout(layoutGeneralSetting);

    QHBoxLayout *layoutButton = new QHBoxLayout;
    layoutButton->addStretch(1);
    layoutButton->addWidget(m_pButtonOk);
    layoutButton->addWidget(m_pButtonApply);
    layoutButton->addWidget(m_pButtonCancel);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_pGroupGeneralSetting);
    layout->addStretch(1);
    layout->addLayout(layoutButton);

    setLayout(layout);

    // connenctions
    connect(m_pButtonOk, &QPushButton::clicked, this, &SettingsWindow::clickedButtonOk);
    connect(m_pButtonApply, &QPushButton::clicked, this, &SettingsWindow::clickedButtonApply);
    connect(m_pButtonCancel, &QPushButton::clicked, this, &SettingsWindow::clickedButtonCancel);
}

void SettingsWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        setWindowTitle(tr("Settings"));

        m_pGroupGeneralSetting->setTitle(tr("General settings"));
        m_pLocaleListLabel->setText(tr("Locale:"));
        m_pPlayerNameLabel->setText(tr("Player name:"));        

        m_pButtonOk->setText(tr("OK"));
        m_pButtonApply->setText(tr("Apply"));
        m_pButtonCancel->setText(tr("Cancel"));
    } else {
        QWidget::changeEvent(event);
    }
}

void SettingsWindow::clickedButtonOk()
{
    clickedButtonApply();
    close();
}

void SettingsWindow::clickedButtonApply()
{
    if (m_pLocaleList->currentData().toString() != Settings::inst().language()) {
        Settings::inst().setLanguage(m_pLocaleList->currentData().toString());
        auto translator = Settings::inst().translator();
        translator->load(":/translations/language_" + Settings::inst().language());
    }

    if (m_pPlayerName->text() != Settings::inst().playerName()) {
        Settings::inst().setPlayerName(m_pPlayerName->text());
    }
}

void SettingsWindow::clickedButtonCancel()
{
//    if (false) {
//        auto answer = QMessageBox::information(this,
//                                               tr("Exit from settings"),
//                                               tr("Are you sure you want to go out? "
//                                                  "Any settings you have changed will not be saved",
//                                                  "Exit from settings"));
//        if (answer == QMessageBox::Cancel)
//            return;
//    }
    close();
}
