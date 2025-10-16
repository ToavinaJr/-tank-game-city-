#include "../include/PowerUp.hpp"
#include "../include/Constants.hpp"
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

// Convert PowerUpType to EntityType
EntityType PowerUp::powerUpTypeToEntityType(PowerUpType type) {
    switch (type) {
    case PowerUpType::HEALTH: return EntityType::HEALTH_POWERUP;
    case PowerUpType::BOMB: return EntityType::BOMB_POWERUP;
    case PowerUpType::SHIELD: return EntityType::SHIELD_POWERUP;
    default: return EntityType::HEALTH_POWERUP;
    }
}

// Convert PowerUpType to QColor
QColor PowerUp::powerUpTypeToColor(PowerUpType type) {
    switch (type) {
    case PowerUpType::HEALTH: return QColor(Colors::HEALTH_POWERUP);
    case PowerUpType::BOMB: return QColor(Colors::BOMB_POWERUP);
    case PowerUpType::SHIELD: return QColor(Colors::SHIELD_POWERUP);
    default: return QColor(Colors::HEALTH_POWERUP);
    }
}

// Update the PowerUp state
void PowerUp::update() {
    if (!m_active) return;

    if (m_lifetime > 0) {
        m_lifetime--;
        m_blinkTimer++;
    } else {
        m_active = false;
    }
}

// Render the PowerUp
void PowerUp::render(QPainter& painter) {
    if (!m_active) return;

    // Blink when about to expire
    if (m_lifetime < 60 && (m_blinkTimer / 10) % 2 == 0) {
        return;
    }

    painter.save();
    painter.setBrush(m_color);
    painter.setPen(QPen(m_color.darker(150), 2));

    const qreal centerX = m_rect.center().x();
    const qreal centerY = m_rect.center().y();

    switch (m_powerUpType) {
    case PowerUpType::HEALTH: {
        // Draw health cross
        painter.drawRect(m_rect);
        painter.setBrush(Qt::white);
        QRectF hRect(centerX - 8, centerY - 2, 16, 4);
        QRectF vRect(centerX - 2, centerY - 8, 4, 16);
        painter.drawRect(hRect);
        painter.drawRect(vRect);
        break;
    }

    case PowerUpType::BOMB: {
        // Draw bomb
        painter.drawEllipse(m_rect);
        painter.setBrush(Qt::black);
        QRectF fuseRect(centerX - 2, m_rect.top(), 4, 8);
        painter.drawRect(fuseRect);
        break;
    }

    case PowerUpType::SHIELD: {
        // Draw shield
        painter.drawEllipse(m_rect);
        painter.setPen(QPen(Qt::white, 3));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(m_rect.adjusted(4, 4, -4, -4));
        break;
    }

    default:
        break;
    }

    painter.restore();
}
