#ifndef BULLET_H
#define BULLET_H

#include "Entity.hpp"

class Bullet : public Entity {
public:
    Bullet(const QPointF& position, Direction direction, bool fromPlayer);
    
    void update() override;
    void render(QPainter& painter) override;
    
    Direction getDirection() const { return m_direction; }
    bool isFromPlayer() const { return m_fromPlayer; }
    
private:
    Direction m_direction;
    int m_speed;
    bool m_fromPlayer;
    
    static constexpr int BULLET_SIZE = 8;
};

#endif // BULLET_H