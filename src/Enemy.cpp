#include "../include/Enemy.hpp"
#include "../include/Constants.hpp"
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
