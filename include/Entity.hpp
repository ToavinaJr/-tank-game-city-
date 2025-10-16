#ifndef ENTITY_H
#define ENTITY_H

#include <QRectF>
#include <QColor>
#include <QPainter>

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum class EntityType {
    PLAYER_TANK,
    ENEMY_TANK,
    BULLET,
    BRICK_BLOCK,
    STEEL_BLOCK,
    WATER_BLOCK,
    TREE_BLOCK,
    BASE,
    HEALTH_POWERUP,
    BOMB_POWERUP,
    SHIELD_POWERUP
};

class Entity {
public:
    Entity(const QRectF& rect, EntityType type, const QColor& color);
    virtual ~Entity() = default;
    
    virtual void update() {}
    virtual void render(QPainter& painter);
    
    QRectF getRect() const { return m_rect; }
    QPointF getPosition() const { return m_rect.topLeft(); }
    EntityType getType() const { return m_type; }
    QColor getColor() const { return m_color; }
    bool isActive() const { return m_active; }
    
    void setPosition(const QPointF& pos);
    void setColor(const QColor& color) { m_color = color; }
    void setActive(bool active) { m_active = active; }
    
    bool collidesWith(const Entity& other) const;
    
protected:
    QRectF m_rect;
    EntityType m_type;
    QColor m_color;
    bool m_active;
};

#endif // ENTITY_H