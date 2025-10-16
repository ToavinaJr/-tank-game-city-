#include "../include/Block.hpp"
#include "../include/Constants.hpp"
#include <QPainter>

Block::Block(const QPointF& position, BlockType blockType)
    : Entity(QRectF(position, QSizeF(BLOCK_SIZE, BLOCK_SIZE)),
             blockTypeToEntityType(blockType),
             blockTypeToColor(blockType))
    , m_blockType(blockType)
{
}

EntityType Block::blockTypeToEntityType(BlockType type) {
    switch (type) {
        case BlockType::BRICK: return EntityType::BRICK_BLOCK;
        case BlockType::STEEL: return EntityType::STEEL_BLOCK;
        case BlockType::WATER: return EntityType::WATER_BLOCK;
        case BlockType::TREE: return EntityType::TREE_BLOCK;
        case BlockType::BASE: return EntityType::BASE;
    }
    return EntityType::BRICK_BLOCK;
}

QColor Block::blockTypeToColor(BlockType type) {
    switch (type) {
        case BlockType::BRICK: return QColor(Colors::BRICK_BLOCK);
        case BlockType::STEEL: return QColor(Colors::STEEL_BLOCK);
        case BlockType::WATER: return QColor(Colors::WATER_BLOCK);
        case BlockType::TREE: return QColor(Colors::TREE_BLOCK);
        case BlockType::BASE: return QColor(Colors::BASE);
    }
    return QColor(Colors::BRICK_BLOCK);
}

bool Block::isDestructible() const {
    return m_blockType == BlockType::BRICK;
}

bool Block::blocksMovement() const {
    return m_blockType != BlockType::TREE;
}

bool Block::isCamouflage() const {
    return m_blockType == BlockType::TREE;
}

void Block::render(QPainter& painter) {
    if (!m_active) return;
    
    painter.save();
    painter.setBrush(m_color);
    painter.setPen(Qt::NoPen);
    
    switch (m_blockType) {
        case BlockType::BRICK:
            // Draw brick pattern
            painter.drawRect(m_rect);
            painter.setPen(QPen(m_color.darker(150), 2));
            for (int i = 0; i < BLOCK_SIZE; i += 8) {
                painter.drawLine(m_rect.left(), m_rect.top() + i, 
                               m_rect.right(), m_rect.top() + i);
                painter.drawLine(m_rect.left() + i, m_rect.top(), 
                               m_rect.left() + i, m_rect.bottom());
            }
            break;
            
        case BlockType::STEEL:
            // Draw steel with diagonal lines
            painter.drawRect(m_rect);
            painter.setPen(QPen(m_color.lighter(120), 2));
            painter.drawLine(m_rect.topLeft(), m_rect.bottomRight());
            painter.drawLine(m_rect.topRight(), m_rect.bottomLeft());
            break;
            
        case BlockType::WATER:
            // Draw wavy water
            painter.drawRect(m_rect);
            painter.setPen(QPen(m_color.lighter(130), 2));
            for (int y = 0; y < BLOCK_SIZE; y += 8) {
                for (int x = 0; x < BLOCK_SIZE; x += 4) {
                    painter.drawPoint(m_rect.left() + x, m_rect.top() + y);
                }
            }
            break;
            
        case BlockType::TREE:
            // Draw tree/foliage
            painter.setOpacity(0.7);
            painter.drawRect(m_rect);
            painter.setOpacity(1.0);
            painter.setBrush(m_color.darker(130));
            for (int i = 0; i < 5; i++) {
                painter.drawEllipse(m_rect.center(), 4 + i * 2, 4 + i * 2);
            }
            break;
            
        case BlockType::BASE:
            // Draw base with eagle symbol
            painter.drawRect(m_rect);
            painter.setBrush(Qt::white);
            QRectF eagleRect = m_rect.adjusted(8, 8, -8, -8);
            painter.drawRect(eagleRect);
            break;
    }
    
    painter.restore();
}
