// HUDWidget.cpp
#include "../include/HUDWidget.hpp"
#include "../include/Constants.hpp"
#include <QVBoxLayout>

HUDWidget::HUDWidget(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(GameConstants::HUD_HEIGHT);
    setupUI();
}

void HUDWidget::setupUI() {
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 5, 10, 5);
    
    // Health section
    QVBoxLayout* healthLayout = new QVBoxLayout();
    m_healthLabel = new QLabel("Health:", this);
    m_healthLabel->setObjectName("hudLabel");
    
    m_healthBar = new QProgressBar(this);
    m_healthBar->setObjectName("healthBar");
    m_healthBar->setMaximum(GameConstants::MAX_PLAYER_HEALTH);
    m_healthBar->setValue(GameConstants::MAX_PLAYER_HEALTH);
    m_healthBar->setTextVisible(true);
    m_healthBar->setFormat("%v / %m");
    
    healthLayout->addWidget(m_healthLabel);
    healthLayout->addWidget(m_healthBar);
    
    // Score section
    QVBoxLayout* scoreLayout = new QVBoxLayout();
    QLabel* scoreTitle = new QLabel("Score:", this);
    scoreTitle->setObjectName("hudLabel");
    m_scoreLabel = new QLabel("0", this);
    m_scoreLabel->setObjectName("hudValue");
    
    scoreLayout->addWidget(scoreTitle);
    scoreLayout->addWidget(m_scoreLabel);
    
    // Level section
    QVBoxLayout* levelLayout = new QVBoxLayout();
    QLabel* levelTitle = new QLabel("Level:", this);
    levelTitle->setObjectName("hudLabel");
    m_levelLabel = new QLabel("1", this);
    m_levelLabel->setObjectName("hudValue");
    
    levelLayout->addWidget(levelTitle);
    levelLayout->addWidget(m_levelLabel);
    
    // Enemies section
    QVBoxLayout* enemiesLayout = new QVBoxLayout();
    QLabel* enemiesTitle = new QLabel("Enemies:", this);
    enemiesTitle->setObjectName("hudLabel");
    m_enemiesLabel = new QLabel("20", this);
    m_enemiesLabel->setObjectName("hudValue");
    
    enemiesLayout->addWidget(enemiesTitle);
    enemiesLayout->addWidget(m_enemiesLabel);
    
    // Add all sections to main layout
    mainLayout->addLayout(healthLayout, 2);
    mainLayout->addLayout(scoreLayout, 1);
    mainLayout->addLayout(levelLayout, 1);
    mainLayout->addLayout(enemiesLayout, 1);
}

void HUDWidget::updateHealth(int health) {
    m_healthBar->setValue(health);
}

void HUDWidget::updateScore(int score) {
    m_scoreLabel->setText(QString::number(score));
}

void HUDWidget::updateLevel(int level) {
    m_levelLabel->setText(QString::number(level));
}

void HUDWidget::updateEnemiesRemaining(int enemies) {
    m_enemiesLabel->setText(QString::number(enemies));
}