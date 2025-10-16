#include "../include/SettingsWidget.hpp"
#include "../include/SaveManager.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPainter>

SettingsWidget::SettingsWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    loadSettings();
}

void SettingsWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(50, 30, 50, 30);
    mainLayout->setSpacing(20);
    
    // Title
    QLabel* titleLabel = new QLabel("Settings", this);
    titleLabel->setObjectName("settingsTitle");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // Sound settings group
    QGroupBox* soundGroup = new QGroupBox("Sound Settings", this);
    soundGroup->setObjectName("settingsGroup");
    QVBoxLayout* soundLayout = new QVBoxLayout(soundGroup);
    
    m_soundCheckBox = new QCheckBox("Enable Sound", this);
    m_soundCheckBox->setObjectName("settingsCheckBox");
    soundLayout->addWidget(m_soundCheckBox);
    
    // Music volume
    QHBoxLayout* musicLayout = new QHBoxLayout();
    QLabel* musicLabel = new QLabel("Music Volume:", this);
    musicLabel->setObjectName("settingsLabel");
    m_musicVolumeSlider = new QSlider(Qt::Horizontal, this);
    m_musicVolumeSlider->setObjectName("settingsSlider");
    m_musicVolumeSlider->setRange(0, 100);
    m_musicVolumeLabel = new QLabel("50", this);
    m_musicVolumeLabel->setObjectName("settingsValueLabel");
    m_musicVolumeLabel->setMinimumWidth(30);
    
    musicLayout->addWidget(musicLabel);
    musicLayout->addWidget(m_musicVolumeSlider, 1);
    musicLayout->addWidget(m_musicVolumeLabel);
    soundLayout->addLayout(musicLayout);
    
    // SFX volume
    QHBoxLayout* sfxLayout = new QHBoxLayout();
    QLabel* sfxLabel = new QLabel("SFX Volume:", this);
    sfxLabel->setObjectName("settingsLabel");
    m_sfxVolumeSlider = new QSlider(Qt::Horizontal, this);
    m_sfxVolumeSlider->setObjectName("settingsSlider");
    m_sfxVolumeSlider->setRange(0, 100);
    m_sfxVolumeLabel = new QLabel("70", this);
    m_sfxVolumeLabel->setObjectName("settingsValueLabel");
    m_sfxVolumeLabel->setMinimumWidth(30);
    
    sfxLayout->addWidget(sfxLabel);
    sfxLayout->addWidget(m_sfxVolumeSlider, 1);
    sfxLayout->addWidget(m_sfxVolumeLabel);
    soundLayout->addLayout(sfxLayout);
    
    mainLayout->addWidget(soundGroup);
    
    // Appearance settings group
    QGroupBox* appearanceGroup = new QGroupBox("Appearance", this);
    appearanceGroup->setObjectName("settingsGroup");
    QVBoxLayout* appearanceLayout = new QVBoxLayout(appearanceGroup);
    
    QHBoxLayout* colorLayout = new QHBoxLayout();
    QLabel* colorLabel = new QLabel("Tank Color:", this);
    colorLabel->setObjectName("settingsLabel");
    m_tankColorButton = new QPushButton("Choose Color", this);
    m_tankColorButton->setObjectName("colorButton");
    m_tankColorButton->setMinimumHeight(40);
    m_tankColorButton->setCursor(Qt::PointingHandCursor);
    
    colorLayout->addWidget(colorLabel);
    colorLayout->addWidget(m_tankColorButton, 1);
    appearanceLayout->addLayout(colorLayout);
    
    mainLayout->addWidget(appearanceGroup);
    
    mainLayout->addStretch();
    
    // Bottom buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    m_saveButton = new QPushButton("Save", this);
    m_saveButton->setObjectName("menuButton");
    m_saveButton->setMinimumSize(150, 45);
    m_saveButton->setCursor(Qt::PointingHandCursor);
    
    m_backButton = new QPushButton("Back", this);
    m_backButton->setObjectName("menuButton");
    m_backButton->setMinimumSize(150, 45);
    m_backButton->setCursor(Qt::PointingHandCursor);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_saveButton);
    buttonLayout->addWidget(m_backButton);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
    
    // Connect signals
    connect(m_soundCheckBox, &QCheckBox::toggled, this, &SettingsWidget::onSoundToggled);
    connect(m_musicVolumeSlider, &QSlider::valueChanged, this, &SettingsWidget::onMusicVolumeChanged);
    connect(m_sfxVolumeSlider, &QSlider::valueChanged, this, &SettingsWidget::onSfxVolumeChanged);
    connect(m_tankColorButton, &QPushButton::clicked, this, &SettingsWidget::onTankColorClicked);
    connect(m_saveButton, &QPushButton::clicked, this, &SettingsWidget::onSaveClicked);
    connect(m_backButton, &QPushButton::clicked, this, &SettingsWidget::onBackClicked);
}

void SettingsWidget::loadSettings() {
    auto& saveManager = SaveManager::instance();
    
    m_soundCheckBox->setChecked(saveManager.getSoundEnabled());
    m_musicVolumeSlider->setValue(saveManager.getMusicVolume());
    m_sfxVolumeSlider->setValue(saveManager.getSfxVolume());
    m_selectedTankColor = saveManager.getTankColor();
    
    m_musicVolumeLabel->setText(QString::number(saveManager.getMusicVolume()));
    m_sfxVolumeLabel->setText(QString::number(saveManager.getSfxVolume()));
    
    // Update color button style
    QString colorStyle = QString("background-color: %1;").arg(m_selectedTankColor.name());
    m_tankColorButton->setStyleSheet(colorStyle);
}

void SettingsWidget::saveSettings() {
    auto& saveManager = SaveManager::instance();
    
    saveManager.setSoundEnabled(m_soundCheckBox->isChecked());
    saveManager.setMusicVolume(m_musicVolumeSlider->value());
    saveManager.setSfxVolume(m_sfxVolumeSlider->value());
    saveManager.setTankColor(m_selectedTankColor);
    
    emit settingsChanged();
}

void SettingsWidget::onSoundToggled(bool checked) {
    m_musicVolumeSlider->setEnabled(checked);
    m_sfxVolumeSlider->setEnabled(checked);
}

void SettingsWidget::onMusicVolumeChanged(int value) {
    m_musicVolumeLabel->setText(QString::number(value));
}

void SettingsWidget::onSfxVolumeChanged(int value) {
    m_sfxVolumeLabel->setText(QString::number(value));
}

void SettingsWidget::onTankColorClicked() {
    QColor newColor = QColorDialog::getColor(m_selectedTankColor, this, "Choose Tank Color");
    
    if (newColor.isValid()) {
        m_selectedTankColor = newColor;
        QString colorStyle = QString("background-color: %1;").arg(m_selectedTankColor.name());
        m_tankColorButton->setStyleSheet(colorStyle);
    }
}

void SettingsWidget::onSaveClicked() {
    saveSettings();
    emit backClicked();
}

void SettingsWidget::onBackClicked() {
    loadSettings(); // Reset to saved values
    emit backClicked();
}
