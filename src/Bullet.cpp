#include "../include/Bullet.hpp"
#include "../include/Constants.hpp"
#include <QPainter>

Bullet::Bullet(const QPointF& position, Direction direction, bool fromPlayer)
    : Entity(QRectF(position, QSizeF(BULLET_SIZE, BULLET_SIZE)),
             EntityType::BULLET,
             fromPlayer ? Qt::yellow : Qt::red)
    , m_direction(direction)
    , m_speed(GameConstants::BULLET_SPEED)
    , m_fromPlayer(fromPlayer)
{
    setActive(true);
}

void Bullet::update() {
    if (!isActive()) return;

    QPointF pos = getPosition();

    switch (m_direction)
    {
        case Direction::UP:    pos.ry() -= m_speed; break;
        case Direction::DOWN:  pos.ry() += m_speed; break;
        case Direction::LEFT:  pos.rx() -= m_speed; break;
        case Direction::RIGHT: pos.rx() += m_speed; break;
    }

    setPosition(pos);

    if (pos.x() < 0 || pos.x() + BULLET_SIZE > GameConstants::GAME_AREA_WIDTH ||
        pos.y() < 0 || pos.y() + BULLET_SIZE > GameConstants::GAME_AREA_HEIGHT) {
        setActive(false);
    }
}

void Bullet::render(QPainter& painter) {
    if (!isActive()) return;

    painter.save();
    painter.setBrush(m_fromPlayer ? Qt::yellow : Qt::red);
    painter.setPen(Qt::NoPen);
    painter.drawRect(getRect());
    painter.restore();
}
