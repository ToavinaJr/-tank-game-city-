#include "../include/Tank.hpp"
#include "../include/Constants.hpp"
#include <QPainter>
#include <QtMath>

Tank::Tank(const QPointF& position, EntityType type, const QColor& color, int speed)
    : Entity(QRectF(position, QSizeF(TANK_SIZE, TANK_SIZE)), type, color)
    , m_direction(Direction::UP)
    , m_speed(speed)
    , m_health(GameConstants::MAX_PLAYER_HEALTH)
    , m_movingUp(false)
    , m_movingDown(false)
    , m_movingLeft(false)
    , m_movingRight(false)
    , m_shieldActive(false)
    , m_shieldTimer(0)
    , m_shootCooldown(0)
{
}

void Tank::update() {
    if (!m_active) return;

    QPointF currentPos = m_rect.topLeft();
    QPointF newPos = currentPos;
    bool attemptedMove = false;

    // Gestion du mouvement - un seul axe à la fois pour mouvement fluide
    if (m_movingUp) {
        newPos.setY(currentPos.y() - m_speed);
        m_direction = Direction::UP;
        attemptedMove = true;
    }
    else if (m_movingDown) {
        newPos.setY(currentPos.y() + m_speed);
        m_direction = Direction::DOWN;
        attemptedMove = true;
    }
    else if (m_movingLeft) {
        newPos.setX(currentPos.x() - m_speed);
        m_direction = Direction::LEFT;
        attemptedMove = true;
    }
    else if (m_movingRight) {
        newPos.setX(currentPos.x() + m_speed);
        m_direction = Direction::RIGHT;
        attemptedMove = true;
    }

    // Appliquer les limites strictes SEULEMENT si on a tenté un mouvement
    if (attemptedMove) {
        // Clamp avec limites précises
        newPos.setX(qMax(0.0, qMin(newPos.x(),
                                   static_cast<double>(GameConstants::GAME_AREA_WIDTH - TANK_SIZE))));
        newPos.setY(qMax(0.0, qMin(newPos.y(),
                                   static_cast<double>(GameConstants::GAME_AREA_HEIGHT - TANK_SIZE))));

        // Appliquer la nouvelle position seulement si elle a changé
        if (newPos != currentPos) {
            m_rect.moveTo(newPos);
        }
    }

    // Gestion du bouclier
    if (m_shieldTimer > 0) {
        m_shieldTimer--;
        if (m_shieldTimer == 0) {
            m_shieldActive = false;
        }
    }

    // Gestion du cooldown de tir
    if (m_shootCooldown > 0) {
        m_shootCooldown--;
    }
}

void Tank::render(QPainter& painter) {
    if (!m_active) return;

    painter.save();

    // Dessiner le bouclier si actif
    if (m_shieldActive) {
        painter.setPen(QPen(QColor(0, 191, 255, 128 + 127 * qSin(m_shieldTimer * 0.2)), 3));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(m_rect.center(), TANK_SIZE * 0.7, TANK_SIZE * 0.7);
    }

    // Dessiner le corps du tank
    painter.setBrush(m_color);
    painter.setPen(QPen(m_color.darker(130), 2));
    painter.drawRect(m_rect);

    // Dessiner des détails sur le tank (chenilles)
    painter.setPen(QPen(m_color.darker(150), 1));
    painter.drawLine(m_rect.left() + 4, m_rect.top(),
                     m_rect.left() + 4, m_rect.bottom());
    painter.drawLine(m_rect.right() - 4, m_rect.top(),
                     m_rect.right() - 4, m_rect.bottom());

    // Dessiner le canon en fonction de la direction
    QRectF barrel;
    QPointF center = m_rect.center();

    painter.setBrush(m_color.darker(120));
    painter.setPen(Qt::NoPen);

    switch (m_direction) {
    case Direction::UP:
        barrel = QRectF(center.x() - BARREL_WIDTH/2, m_rect.top() - BARREL_LENGTH,
                        BARREL_WIDTH, BARREL_LENGTH + TANK_SIZE/2);
        break;
    case Direction::DOWN:
        barrel = QRectF(center.x() - BARREL_WIDTH/2, center.y(),
                        BARREL_WIDTH, BARREL_LENGTH + TANK_SIZE/2);
        break;
    case Direction::LEFT:
        barrel = QRectF(m_rect.left() - BARREL_LENGTH, center.y() - BARREL_WIDTH/2,
                        BARREL_LENGTH + TANK_SIZE/2, BARREL_WIDTH);
        break;
    case Direction::RIGHT:
        barrel = QRectF(center.x(), center.y() - BARREL_WIDTH/2,
                        BARREL_LENGTH + TANK_SIZE/2, BARREL_WIDTH);
        break;
    }

    painter.drawRect(barrel);

    // Dessiner une tourelle au centre
    painter.setBrush(m_color.lighter(110));
    painter.drawEllipse(center, TANK_SIZE / 4, TANK_SIZE / 4);

    painter.restore();
}

void Tank::setMoving(Direction dir, bool moving) {
    switch (dir) {
    case Direction::UP:
        m_movingUp = moving;
        if (moving) {
            m_movingDown = false;  // Empêcher les mouvements opposés
        }
        break;
    case Direction::DOWN:
        m_movingDown = moving;
        if (moving) {
            m_movingUp = false;
        }
        break;
    case Direction::LEFT:
        m_movingLeft = moving;
        if (moving) {
            m_movingRight = false;
        }
        break;
    case Direction::RIGHT:
        m_movingRight = moving;
        if (moving) {
            m_movingLeft = false;
        }
        break;
    }
}

void Tank::takeDamage(int damage) {
    if (m_shieldActive) return;

    m_health -= damage;
    if (m_health < 0) m_health = 0;
    if (m_health == 0) m_active = false;
}

void Tank::heal(int amount) {
    m_health += amount;
    if (m_health > GameConstants::MAX_PLAYER_HEALTH) {
        m_health = GameConstants::MAX_PLAYER_HEALTH;
    }
}

void Tank::activateShield(int duration) {
    m_shieldActive = true;
    m_shieldTimer = duration;
}

bool Tank::canShoot() const {
    return m_shootCooldown == 0;
}

void Tank::resetShootCooldown() {
    m_shootCooldown = SHOOT_COOLDOWN_MAX;
}
