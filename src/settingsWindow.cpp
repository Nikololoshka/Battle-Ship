#include "settingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog (parent)
{
    // initialization
    m_pLocaleListLabel = new QLabel(tr("Locale:"));
    m_pLocaleListComboBox = new QComboBox();

    m_pPlayerNameLabel = new QLabel(tr("Player name:"));
    m_pPlayerNameLineEdit = new QLineEdit();

    m_pAnimationSpeedLabel = new QLabel(tr("Animation speed:"));
    m_pAnimationSpeedSlider = new QSlider(Qt::Horizontal);
    m_pAnimationSpeedSpinBox = new QSpinBox();

    m_pButtonOk = new QPushButton(tr("OK"));
    m_pButtonApply = new QPushButton(tr("Apply"));
    m_pButtonCancel =  new QPushButton(tr("Cancel"));

    // settings
    setMinimumSize(400, 300);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Settings"));

    m_pLocaleListComboBox->addItem("English", "en");
    m_pLocaleListComboBox->addItem("Русский", "ru");

    for (int i = 0; i < m_pLocaleListComboBox->count(); ++i) {
       if (m_pLocaleListComboBox->itemData(i).toString() == Settings::inst().language()) {
           m_pLocaleListComboBox->setCurrentIndex(i);
           break;
       }
    }

    m_pPlayerNameLineEdit->setText(Settings::inst().playerName());

    m_pAnimationSpeedSlider->setRange(200, 2000);
    m_pAnimationSpeedSpinBox->setRange(200, 2000);
    m_pAnimationSpeedSlider->setValue(Settings::inst().animationDelay());
    m_pAnimationSpeedSpinBox->setValue(Settings::inst().animationDelay());
    m_pAnimationSpeedSpinBox->setSuffix(tr("ms"));

    // layout setup
    QHBoxLayout *layoutLocale = new QHBoxLayout;
    layoutLocale->addWidget(m_pLocaleListLabel);
    layoutLocale->addWidget(m_pLocaleListComboBox);

    QHBoxLayout *layoutPlayerName = new QHBoxLayout;
    layoutPlayerName->addWidget(m_pPlayerNameLabel);
    layoutPlayerName->addWidget(m_pPlayerNameLineEdit);

    QVBoxLayout *layoutGeneralSetting = new QVBoxLayout;
    layoutGeneralSetting->addLayout(layoutLocale);
    layoutGeneralSetting->addLayout(layoutPlayerName);

    m_pGroupGeneralSetting = new QGroupBox(tr("General settings"));
    m_pGroupGeneralSetting->setLayout(layoutGeneralSetting);

    QHBoxLayout *layoutAnimationSpeed = new QHBoxLayout;
    layoutAnimationSpeed->addWidget(m_pAnimationSpeedLabel);
    layoutAnimationSpeed->addWidget(m_pAnimationSpeedSlider);
    layoutAnimationSpeed->addWidget(m_pAnimationSpeedSpinBox);

    QVBoxLayout *layoutGameSetting = new QVBoxLayout;
    layoutGameSetting->addLayout(layoutAnimationSpeed);

    m_pGroupGameSetting = new QGroupBox(tr("Game settings"));
    m_pGroupGameSetting->setLayout(layoutGameSetting);

    QHBoxLayout *layoutButton = new QHBoxLayout;
    layoutButton->addStretch(1);
    layoutButton->addWidget(m_pButtonOk);
    layoutButton->addWidget(m_pButtonApply);
    layoutButton->addWidget(m_pButtonCancel);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_pGroupGeneralSetting);
    layout->addWidget(m_pGroupGameSetting);
    layout->addStretch(1);
    layout->addLayout(layoutButton);

    setLayout(layout);

    // connenctions
    connect(m_pButtonOk, &QPushButton::clicked, this, &SettingsWindow::clickedButtonOk);
    connect(m_pButtonApply, &QPushButton::clicked, this, &SettingsWindow::clickedButtonApply);
    connect(m_pButtonCancel, &QPushButton::clicked, this, &SettingsWindow::clickedButtonCancel);
    connect(m_pAnimationSpeedSlider, &QSlider::valueChanged, m_pAnimationSpeedSpinBox, &QSpinBox::setValue);
    connect(m_pAnimationSpeedSpinBox, SIGNAL(valueChanged(int)), m_pAnimationSpeedSlider, SLOT(setValue(int)));
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
    if (m_pLocaleListComboBox->currentData().toString() != Settings::inst().language()) {
        Settings::inst().setLanguage(m_pLocaleListComboBox->currentData().toString());
        auto translator = Settings::inst().translator();
        translator->load(":/translations/language_" + Settings::inst().language());
    }

    bool isChange = false;
    if (m_pPlayerNameLineEdit->text() != Settings::inst().playerName()) {
        Settings::inst().setPlayerName(m_pPlayerNameLineEdit->text());
        isChange = true;
    }

    if (m_pAnimationSpeedSlider->value() != Settings::inst().animationDelay()) {
        Settings::inst().setAnimationDelay(m_pAnimationSpeedSlider->value());
        isChange = true;
    }

    if (isChange) {
        SettingsChangeEvent *event = new SettingsChangeEvent();
        QApplication::postEvent(this->parent(), event);
    }
}

void SettingsWindow::clickedButtonCancel()
{
    if (m_pLocaleListComboBox->currentData().toString() != Settings::inst().language() ||
            m_pPlayerNameLineEdit->text() != Settings::inst().playerName() ||
            m_pAnimationSpeedSlider->value() != Settings::inst().animationDelay()) {
        auto answer = QMessageBox::question(this, tr("Exit from settings"),
                                            tr("Are you sure you want to go out? "
                                               "Any settings you have changed will not be saved",
                                               "Exit from settings"));
        if (answer == QMessageBox::No)
            return;
    }
    close();
}
