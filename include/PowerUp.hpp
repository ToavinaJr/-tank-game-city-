#ifndef POWERUP_H
#define POWERUP_H

#include "Entity.hpp"

enum class PowerUpType {
    HEALTH,
    BOMB,
    SHIELD
};

class PowerUp : public Entity {
public:
    PowerUp(const QPointF& position, PowerUpType powerUpType);
    
    void render(QPainter& painter) override;
    void update() override;
    
    PowerUpType getPowerUpType() const { return m_powerUpType; }
    
private:
    PowerUpType m_powerUpType;
    int m_lifetime;
    int m_blinkTimer;
    
    static constexpr int POWERUP_SIZE = 24;
    static constexpr int MAX_LIFETIME = 300;
    
    EntityType powerUpTypeToEntityType(PowerUpType type);
    QColor powerUpTypeToColor(PowerUpType type);
};

#endif // POWERUP_H