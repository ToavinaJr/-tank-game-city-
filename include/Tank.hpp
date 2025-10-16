#ifndef TANK_H
#define TANK_H

#include "Entity.hpp"
#include "Constants.hpp"
#include <QKeyEvent>

class Tank : public Entity {
public:
    Tank(const QPointF& position, EntityType type, const QColor& color, int speed);
    
    void update() override;
    void render(QPainter& painter) override;
    
    void move(Direction dir);
    void setMoving(Direction dir, bool moving);
    Direction getDirection() const { return m_direction; }
    
    int getHealth() const { return m_health; }
    void setHealth(int health) { m_health = health; }
    void takeDamage(int damage);
    void heal(int amount);
    
    bool hasShield() const { return m_shieldActive; }
    void activateShield(int duration);
    
    bool canShoot() const;
    void resetShootCooldown();
    
private:
    Direction m_direction;
    int m_speed;
    int m_health;
    bool m_movingUp;
    bool m_movingDown;
    bool m_movingLeft;
    bool m_movingRight;
    bool m_shieldActive;
    int m_shieldTimer;
    int m_shootCooldown;
    
    static constexpr int SHOOT_COOLDOWN_MAX = 30;
    static constexpr int TANK_SIZE = 28;
    static constexpr int BARREL_LENGTH = 12;
    static constexpr int BARREL_WIDTH = 6;
};

#endif // TANK_H