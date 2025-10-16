// PowerUp.cpp
#include "PowerUp.h"
#include "Constants.h"
#include <QPainter>

PowerUp::PowerUp(const QPointF& position, PowerUpType powerUpType)
    : Entity(QRectF(position, QSizeF(POWERUP_SIZE, POWERUP_SIZE)),
             powerUpTypeToEntityType(powerUpType),
             powerUpTypeToColor(powerUpType))
    , m_powerUpType(powerUpType)
    , m_lifetime(MAX_LIFETIME)
    , m_blinkTimer(0)
{
}

EntityType PowerUp::powerUpTypeToEntityType(PowerUpType type) {
    switch (type) {
        case PowerUpType::HEALTH: return EntityType::HEALTH_POWERUP;
        case PowerUpType::BOMB: return EntityType::BOMB_POWERUP;
        case PowerUpType::SHIELD: return EntityType::SHIELD_POWERUP;
    }
    return EntityType::HEALTH_POWERUP;
}

QColor PowerUp::powerUpTypeToColor(PowerUpType type) {
    switch (type) {
        case PowerUpType::HEALTH: return QColor(Colors::HEALTH_POWERUP);
        case PowerUpType::BOMB: return QColor(Colors::BOMB_POWERUP);
        case PowerUpType::SHIELD: return QColor(Colors::SHIELD_POWERUP);
    }
    return QColor(Colors::HEALTH_POWERUP);
}

void PowerUp::update() {
    if (!m_active) return;
    
    m_lifetime--;
    if (m_lifetime <= 0) {
        m_active = false;
        return;
    }
    
    m_blinkTimer++;
}

void PowerUp::render(QPainter& painter) {
    if (!m_active) return;
    
    // Blink when about to expire
    if (m_lifetime < 60 && (m_blinkTimer / 10) % 2 == 0) {
        return;
    }
    
    painter.save();
    painter.setBrush(m_color);
    painter.setPen(QPen(m_color.darker(150), 2));
    
    switch (m_powerUpType) {
        case PowerUpType::HEALTH:
            // Draw health cross
            painter.drawRect(m_rect);
            painter.setBrush(Qt::white);
            QRectF hRect(m_rect.center().x() - 8, m_rect.center().y() - 2, 16, 4);
            QRectF vRect(m_rect.center().x() - 2, m_rect.center().y() - 8, 4, 16);
            painter.drawRect(hRect);
            painter.drawRect(vRect);
            break;
            
        case PowerUpType::BOMB:
            // Draw bomb
            painter.drawEllipse(m_rect);
            painter.setBrush(Qt::black);
            painter.drawRect(m_rect.center().x() - 2, m_rect.top(), 4, 8);
            break;
            
        case PowerUpType::SHIELD:
            // Draw shield
            painter.drawEllipse(m_rect);
            painter.setPen(QPen(Qt::white, 3));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(m_rect.adjusted(4, 4, -4, -4));
            break;
    }
    
    painter.restore();
}

// Enemy.cpp
#include "Enemy.h"
#include "Constants.h"
#include <QRandomGenerator>

Enemy::Enemy(const QPointF& position)
    : Tank(position, EntityType::ENEMY_TANK, QColor(Colors::ENEMY_TANK), 
           GameConstants::ENEMY_SPEED)
    , m_aiTimer(0)
    , m_shootTimer(0)
    , m_directionChangeTimer(DIRECTION_CHANGE_INTERVAL)
{
    setHealth(1);
}

void Enemy::update() {
    Tank::update();
    
    m_aiTimer++;
    m_shootTimer++;
    m_directionChangeTimer--;
    
    if (m_aiTimer >= AI_UPDATE_INTERVAL) {
        m_aiTimer = 0;
        updateAI();
    }
}

void Enemy::updateAI() {
    if (m_directionChangeTimer <= 0) {
        Direction newDir = getRandomDirection();
        setMoving(getDirection(), false);
        setMoving(newDir, true);
        m_directionChangeTimer = DIRECTION_CHANGE_INTERVAL + 
                                 QRandomGenerator::global()->bounded(60);
    }
}

bool Enemy::shouldShoot() const {
    return m_shootTimer >= SHOOT_INTERVAL && canShoot();
}

Direction Enemy::getRandomDirection() {
    int random = QRandomGenerator::global()->bounded(4);
    switch (random) {
        case 0: return Direction::UP;
        case 1: return Direction::DOWN;
        case 2: return Direction::LEFT;
        case 3: return Direction::RIGHT;
    }
    return Direction::UP;
}