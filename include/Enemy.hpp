// Enemy.h
#ifndef ENEMY_H
#define ENEMY_H

#include "Tank.hpp"
#include <QTimer>

class Enemy : public Tank {
public:
    Enemy(const QPointF& position);
    
    void update() override;
    void updateAI();
    
    bool shouldShoot() const;
    void resetShootTimer() { m_shootTimer = 0; }
    
private:
    int m_aiTimer;
    int m_shootTimer;
    int m_directionChangeTimer;
    
    static constexpr int AI_UPDATE_INTERVAL = 30;
    static constexpr int SHOOT_INTERVAL = 120;
    static constexpr int DIRECTION_CHANGE_INTERVAL = 60;
    
    Direction getRandomDirection();
};

#endif // ENEMY_H