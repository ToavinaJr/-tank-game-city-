#ifndef BLOCK_H
#define BLOCK_H

#include "Entity.hpp"

enum class BlockType {
    BRICK,      // Destructible
    STEEL,      // Indestructible
    WATER,      // Blocks movement
    TREE,       // Camouflage - doesn't block
    BASE        // Player base to protect
};

class Block : public Entity {
public:
    Block(const QPointF& position, BlockType blockType);
    
    void render(QPainter& painter) override;
    
    BlockType getBlockType() const { return m_blockType; }
    bool isDestructible() const;
    bool blocksMovement() const;
    bool isCamouflage() const;
    
private:
    BlockType m_blockType;
    
    static constexpr int BLOCK_SIZE = 32;
    
    EntityType blockTypeToEntityType(BlockType type);
    QColor blockTypeToColor(BlockType type);
};

#endif // BLOCK_H