#include "../include/MenuWidget.hpp"
#include <QFont>
#include <QSpacerItem>
#include <QDebug>

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget(parent)
{
    // --- Titre du jeu ---
    m_titleLabel = new QLabel("Tank Battle City", this);
    QFont titleFont("Arial", 24, QFont::Bold);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);

    // --- Boutons ---
    m_startButton = new QPushButton("Start Game", this);
    m_settingsButton = new QPushButton("Settings", this);
    m_quitButton = new QPushButton("Quit", this);

    // --- Layout vertical ---
    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(m_titleLabel);
    m_layout->addSpacing(20);
    m_layout->addWidget(m_startButton);
    m_layout->addWidget(m_settingsButton);
    m_layout->addWidget(m_quitButton);
    m_layout->addStretch();
    setLayout(m_layout);

    // --- Connexions ---
    connect(m_startButton, &QPushButton::clicked, this, &MenuWidget::onStartClicked);
    connect(m_settingsButton, &QPushButton::clicked, this, &MenuWidget::onSettingsClicked);
    connect(m_quitButton, &QPushButton::clicked, this, &MenuWidget::onQuitClicked);

    // --- Style basique ---
    setStyleSheet(
        "QWidget { background-color: #101820; }"
        "QLabel { color: #FEE715; font-size: 28px; }"
        "QPushButton { "
        "   background-color: #FEE715; "
        "   color: #101820; "
        "   border-radius: 6px; "
        "   padding: 8px 20px; "
        "   font-size: 16px; "
        "} "
        "QPushButton:hover { "
        "   background-color: #fff13b; "
        "} "
        "QPushButton:pressed { "
        "   background-color: #cdbf12; "
        "}"
        );
}

// --- Slots ---
void MenuWidget::onStartClicked()
{
    emit startGame();
}

void MenuWidget::onSettingsClicked()
{
    emit openSettings();
}

void MenuWidget::onQuitClicked()
{
    emit quitGame();
}
