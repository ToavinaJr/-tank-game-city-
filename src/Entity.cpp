#include "../include/Entity.hpp"

Entity::Entity(const QRectF& rect, EntityType type, const QColor& color)
    : m_rect(rect)
    , m_type(type)
    , m_color(color)
    , m_active(true)
{
}

void Entity::render(QPainter& painter) {
    if (!m_active) return;
    
    painter.save();
    painter.setBrush(m_color);
    painter.setPen(Qt::NoPen);
    painter.drawRect(m_rect);
    painter.restore();
}

void Entity::setPosition(const QPointF& pos) {
    m_rect.moveTo(pos);
}

bool Entity::collidesWith(const Entity& other) const {
    if (!m_active || !other.isActive()) return false;
    return m_rect.intersects(other.getRect());
}