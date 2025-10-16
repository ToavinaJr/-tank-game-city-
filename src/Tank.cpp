#include "../include/Tank.hpp"
#include <QPainter>
#include <cmath>

Tank::Tank(const QPointF& position, EntityType type, const QColor& color, int speed)
    : Entity(QRectF(position.x(), position.y(), TANK_SIZE, TANK_SIZE), type, color),
    m_direction(Direction::UP),
    m_speed(speed),
    m_health(100),
    m_movingUp(false),
    m_movingDown(false),
    m_movingLeft(false),
    m_movingRight(false),
    m_shieldActive(false),
    m_shieldTimer(0),
    m_shootCooldown(0)
{
    // rien à ajouter, tout est bien initialisé
}


void Tank::update()
{
    if (m_movingUp)    m_rect.translate(0, -m_speed);
    if (m_movingDown)  m_rect.translate(0,  m_speed);
    if (m_movingLeft)  m_rect.translate(-m_speed, 0);
    if (m_movingRight) m_rect.translate( m_speed, 0);

    if (m_shieldActive && m_shieldTimer > 0) {
        m_shieldTimer--;
        if (m_shieldTimer <= 0)
            m_shieldActive = false;
    }

    // Cooldown de tir
    if (m_shootCooldown > 0)
        m_shootCooldown--;
}

void Tank::render(QPainter& painter)
{
    // Corps du tank
    painter.setBrush(m_color);
    painter.setPen(Qt::NoPen);
    painter.drawRect(m_rect);

    // Canon
    const QPointF center = m_rect.center();
    constexpr double BARREL_WIDTH = 6.0;
    constexpr double BARREL_LENGTH = 18.0;

    QRectF barrelRect;

    switch (m_direction) {
    case Direction::UP:
        barrelRect = QRectF(center.x() - BARREL_WIDTH / 2,
                            m_rect.top() - BARREL_LENGTH / 2,
                            BARREL_WIDTH,
                            BARREL_LENGTH);
        break;
    case Direction::DOWN:
        barrelRect = QRectF(center.x() - BARREL_WIDTH / 2,
                            m_rect.bottom() - BARREL_LENGTH / 2,
                            BARREL_WIDTH,
                            BARREL_LENGTH);
        break;
    case Direction::LEFT:
        barrelRect = QRectF(m_rect.left() - BARREL_LENGTH / 2,
                            center.y() - BARREL_WIDTH / 2,
                            BARREL_LENGTH,
                            BARREL_WIDTH);
        break;
    case Direction::RIGHT:
        barrelRect = QRectF(m_rect.right() - BARREL_LENGTH / 2,
                            center.y() - BARREL_WIDTH / 2,
                            BARREL_LENGTH,
                            BARREL_WIDTH);
        break;
    }

    painter.fillRect(barrelRect, Qt::darkGray);

    // Bouclier actif
    if (m_shieldActive) {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(Qt::cyan, 2));
        painter.drawEllipse(m_rect.adjusted(-5, -5, 5, 5));
    }
}


void Tank::move(Direction dir)
{
    // Mettre à jour la direction
    m_direction = dir;

    // Déplacement selon la direction
    switch (dir) {
    case Direction::UP:    m_rect.translate(0, -m_speed); break;
    case Direction::DOWN:  m_rect.translate(0,  m_speed); break;
    case Direction::LEFT:  m_rect.translate(-m_speed, 0); break;
    case Direction::RIGHT: m_rect.translate( m_speed, 0); break;
    }
}


void Tank::setMoving(Direction dir, bool moving)
{
    if (moving)
        m_direction = dir;

    switch (dir) {
    case Direction::UP:    m_movingUp    = moving; break;
    case Direction::DOWN:  m_movingDown  = moving; break;
    case Direction::LEFT:  m_movingLeft  = moving; break;
    case Direction::RIGHT: m_movingRight = moving; break;
    }
}


void Tank::takeDamage(int damage)
{
    if (!m_shieldActive) {
        m_health -= std::abs(damage);
        if (m_health < 0)
            m_health = 0;
    }
}

void Tank::heal(int amount)
{
    m_health += std::abs(amount);
}


void Tank::activateShield(int duration)
{
    m_shieldActive = true;
    m_shieldTimer = duration;
}


bool Tank::canShoot() const
{
    return m_shootCooldown <= 0;
}

 void Tank::resetShootCooldown()
{
    m_shootCooldown = SHOOT_COOLDOWN_MAX;
}
