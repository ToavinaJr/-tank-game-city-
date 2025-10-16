#include "../include/Bullet.hpp"
#include "../include/Constants.hpp"
#include <QPainter>

Bullet::Bullet(const QPointF& position, Direction direction, bool fromPlayer)
    : Entity(QRectF(position, QSizeF(BULLET_SIZE, BULLET_SIZE)),
             EntityType::BULLET,
             QColor(Colors::BULLET))
    , m_direction(direction)
    , m_speed(GameConstants::BULLET_SPEED)
    , m_fromPlayer(fromPlayer)
{
    // Ajuster la position initiale pour centrer la balle sur le canon
    QPointF adjustedPos = position;
    adjustedPos.setX(adjustedPos.x() - BULLET_SIZE / 2);
    adjustedPos.setY(adjustedPos.y() - BULLET_SIZE / 2);
    m_rect.moveTo(adjustedPos);
}

void Bullet::update() {
    if (!m_active) return;

    QPointF currentPos = m_rect.topLeft();
    QPointF newPos = currentPos;

    // Mouvement progressif dans la direction
    switch (m_direction) {
    case Direction::UP:
        newPos.setY(currentPos.y() - m_speed);
        break;
    case Direction::DOWN:
        newPos.setY(currentPos.y() + m_speed);
        break;
    case Direction::LEFT:
        newPos.setX(currentPos.x() - m_speed);
        break;
    case Direction::RIGHT:
        newPos.setX(currentPos.x() + m_speed);
        break;
    }

    // Vérifier les limites avec marge généreuse
    const int MARGIN = 50;
    if (newPos.x() < -MARGIN ||
        newPos.x() > GameConstants::GAME_AREA_WIDTH + MARGIN ||
        newPos.y() < -MARGIN ||
        newPos.y() > GameConstants::GAME_AREA_HEIGHT + MARGIN) {
        m_active = false;
        return;
    }

    // Appliquer le mouvement
    m_rect.moveTo(newPos);
}

void Bullet::render(QPainter& painter) {
    if (!m_active) return;

    painter.save();

    // Dessiner une balle plus visible avec un effet brillant
    painter.setBrush(m_color);
    painter.setPen(QPen(m_color.lighter(150), 1));

    // Cercle principal
    painter.drawEllipse(m_rect);

    // Point lumineux au centre
    QPointF center = m_rect.center();
    painter.setBrush(Qt::white);
    painter.drawEllipse(center, BULLET_SIZE / 4, BULLET_SIZE / 4);

    painter.restore();
}
