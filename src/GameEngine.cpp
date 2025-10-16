#include "../include/GameEngine.hpp"
#include "../include/Constants.hpp"
#include "../include/GameConfig.hpp"
#include <QRandomGenerator>
#include <algorithm>

GameEngine::GameEngine(QObject* parent)
    : QObject(parent)
    , m_state(GameState::MENU)
    , m_score(0)
    , m_level(1)
    , m_enemiesRemaining(0)
    , m_activeEnemies(0)
    , m_baseDestroyed(false)
{
    m_gameTimer = new QTimer(this);
    m_gameTimer->setInterval(GameConstants::GAME_TICK_INTERVAL);
    connect(m_gameTimer, &QTimer::timeout, this, &GameEngine::update);
    
    m_enemySpawnTimer = new QTimer(this);
    m_enemySpawnTimer->setInterval(GameConstants::ENEMY_SPAWN_INTERVAL);
    connect(m_enemySpawnTimer, &QTimer::timeout, this, &GameEngine::spawnEnemy);
}

GameEngine::~GameEngine() = default;

void GameEngine::startGame() {
    m_state = GameState::PLAYING;
    m_score = 0;
    m_level = 1;
    m_baseDestroyed = false;
    
    initializeLevel();
    
    m_gameTimer->start();
    m_enemySpawnTimer->start();
    
    emit gameStateChanged(m_state);
    emit scoreChanged(m_score);
    emit levelChanged(m_level);
}

void GameEngine::initializeLevel() {
    m_enemies.clear();
    m_bullets.clear();
    m_blocks.clear();
    m_powerUps.clear();
    
    // Create player
    QPointF playerStart( 16,
                        GameConstants::GAME_AREA_HEIGHT - 50);
    m_player = std::make_unique<Tank>(playerStart, EntityType::PLAYER_TANK,
                                      GameConfig::instance().getTankColor(),
                                      GameConstants::PLAYER_SPEED);
    
    m_enemiesRemaining = GameConstants::MAX_ENEMIES;
    m_activeEnemies = 0;
    
    createLevel();
    
    emit playerHealthChanged(m_player->getHealth());
}

void GameEngine::createLevel() {
    // Create base in bottom center
    QPointF basePos(GameConstants::GAME_AREA_WIDTH / 2 - 16, 
                    GameConstants::GAME_AREA_HEIGHT - 64);
    m_blocks.push_back(std::make_unique<Block>(basePos, BlockType::BASE));
    
    // Create protective walls around base
    for (int i = -2; i <= 2; i++) {
        if (i == 0) continue;
        QPointF pos(basePos.x() + i * GameConstants::CELL_SIZE, basePos.y() - 32);
        m_blocks.push_back(std::make_unique<Block>(pos, BlockType::BRICK));
    }
    
    // Create random level layout
    for (int y = 0; y < GameConstants::GRID_HEIGHT - 3; y++) {
        for (int x = 0; x < GameConstants::GRID_WIDTH; x++) {
            if (QRandomGenerator::global()->bounded(100) < 30) {
                QPointF pos(x * GameConstants::CELL_SIZE, y * GameConstants::CELL_SIZE);
                
                int blockChoice = QRandomGenerator::global()->bounded(100);
                if (blockChoice < 60) {
                    m_blocks.push_back(std::make_unique<Block>(pos, BlockType::BRICK));
                } else if (blockChoice < 75) {
                    m_blocks.push_back(std::make_unique<Block>(pos, BlockType::STEEL));
                } else if (blockChoice < 85) {
                    m_blocks.push_back(std::make_unique<Block>(pos, BlockType::WATER));
                } else {
                    m_blocks.push_back(std::make_unique<Block>(pos, BlockType::TREE));
                }
            }
        }
    }
}

void GameEngine::update() {
    if (m_state != GameState::PLAYING) return;
    
    // Update player
    QPointF oldPos = m_player->getPosition();
    m_player->update();
    if (!isValidMove(m_player->getRect(), m_player.get())) {
        m_player->setPosition(oldPos);
    }
    
    // Update enemies
    updateEnemies();
    
    // Update bullets
    for (auto& bullet : m_bullets) {
        bullet->update();
    }
    
    // Update power-ups
    for (auto& powerUp : m_powerUps) {
        powerUp->update();
    }
    
    checkCollisions();
    cleanupInactive();
    
    // Check win condition
    if (m_enemiesRemaining == 0 && m_enemies.empty()) {
        m_state = GameState::LEVEL_COMPLETE;
        m_gameTimer->stop();
        m_enemySpawnTimer->stop();
        emit gameStateChanged(m_state);
    }
    
    // Check lose condition
    if (!m_player->isActive() || m_baseDestroyed) {
        m_state = GameState::GAME_OVER;
        m_gameTimer->stop();
        m_enemySpawnTimer->stop();
        emit gameStateChanged(m_state);
    }
}

void GameEngine::updateEnemies() {
    for (auto& enemy : m_enemies) {
        QPointF oldPos = enemy->getPosition();
        enemy->update();
        
        if (!isValidMove(enemy->getRect(), enemy.get())) {
            enemy->setPosition(oldPos);
        }
        
        // Enemy shooting
        if (enemy->shouldShoot()) {
            QPointF bulletPos = enemy->getRect().center();
            m_bullets.push_back(std::make_unique<Bullet>(bulletPos, 
                                                         enemy->getDirection(), 
                                                         false));
            enemy->resetShootCooldown();
            enemy->resetShootTimer();
            emit soundEffect("enemy_shoot");
        }
    }
}

QPointF GameEngine::getSpawnPosition(int index) {
    int spawnPoints = 3;
    int spacing = GameConstants::GAME_AREA_WIDTH / (spawnPoints + 1);
    int x = spacing * ((index % spawnPoints) + 1) - 16;
    return QPointF(x, 16);
}

void GameEngine::spawnEnemy() {
    if (m_state != GameState::PLAYING) return;
    
    if (m_activeEnemies < GameConstants::ACTIVE_ENEMIES && m_enemiesRemaining > 0) {
        QPointF spawnPos = getSpawnPosition(m_activeEnemies);
        m_enemies.push_back(std::make_unique<Enemy>(spawnPos));
        m_enemiesRemaining--;
        m_activeEnemies++;
        emit soundEffect("enemy_spawn");
    }
}

void GameEngine::checkCollisions() {
    checkBulletCollisions();
    checkPowerUpCollisions();
}

void GameEngine::checkBulletCollisions() {
    for (auto& bullet : m_bullets) {
        if (!bullet->isActive()) continue;
        
        // Check bullet vs blocks
        for (auto& block : m_blocks) {
            if (!block->isActive()) continue;
            if (bullet->collidesWith(*block)) {
                Block* blockPtr = static_cast<Block*>(block.get());
                
                if (blockPtr->getBlockType() == BlockType::BASE) {
                    m_baseDestroyed = true;
                    emit soundEffect("base_destroyed");
                }
                
                if (blockPtr->isDestructible()) {
                    block->setActive(false);
                    emit soundEffect("block_destroyed");
                }
                
                if (!blockPtr->isCamouflage()) {
                    bullet->setActive(false);
                }
            }
        }
        
        // Check bullet vs tanks
        if (bullet->isFromPlayer()) {
            // Player bullet vs enemies
            for (auto& enemy : m_enemies) {
                if (enemy->isActive() && bullet->collidesWith(*enemy)) {
                    enemy->takeDamage(1);
                    bullet->setActive(false);
                    
                    if (!enemy->isActive()) {
                        m_score += GameConstants::ENEMY_KILL_SCORE;
                        m_activeEnemies--;
                        emit scoreChanged(m_score);
                        emit soundEffect("enemy_destroyed");
                        
                        // Random power-up drop
                        if (QRandomGenerator::global()->bounded(100) < 20) {
                            spawnPowerUp();
                        }
                    }
                }
            }
        } else {
            // Enemy bullet vs player
            if (m_player->isActive() && bullet->collidesWith(*m_player)) {
                m_player->takeDamage(1);
                bullet->setActive(false);
                emit playerHealthChanged(m_player->getHealth());
                emit soundEffect("player_hit");
            }
        }
    }
    
    // Check bullet vs bullet
    for (size_t i = 0; i < m_bullets.size(); i++) {
        for (size_t j = i + 1; j < m_bullets.size(); j++) {
            if (m_bullets[i]->isActive() && m_bullets[j]->isActive()) {
                if (m_bullets[i]->collidesWith(*m_bullets[j])) {
                    m_bullets[i]->setActive(false);
                    m_bullets[j]->setActive(false);
                }
            }
        }
    }
}

void GameEngine::checkPowerUpCollisions() {
    for (auto& powerUp : m_powerUps) {
        if (!powerUp->isActive()) continue;
        
        if (powerUp->collidesWith(*m_player)) {
            PowerUp* powerUpPtr = static_cast<PowerUp*>(powerUp.get());
            
            switch (powerUpPtr->getPowerUpType()) {
                case PowerUpType::HEALTH:
                    m_player->heal(1);
                    emit playerHealthChanged(m_player->getHealth());
                    emit soundEffect("powerup_health");
                    break;
                    
                case PowerUpType::BOMB:
                    triggerBomb();
                    emit soundEffect("powerup_bomb");
                    break;
                    
                case PowerUpType::SHIELD:
                    m_player->activateShield(300);
                    emit soundEffect("powerup_shield");
                    break;
            }
            
            powerUp->setActive(false);
            m_score += 50;
            emit scoreChanged(m_score);
        }
    }
}

void GameEngine::spawnPowerUp() {
    // Find random empty position
    QPointF pos;
    bool validPos = false;
    int attempts = 0;
    
    while (!validPos && attempts < 50) {
        int x = QRandomGenerator::global()->bounded(GameConstants::GRID_WIDTH) * 
                GameConstants::CELL_SIZE;
        int y = QRandomGenerator::global()->bounded(GameConstants::GRID_HEIGHT) * 
                GameConstants::CELL_SIZE;
        pos = QPointF(x, y);
        
        QRectF testRect(pos, QSizeF(24, 24));
        validPos = isValidMove(testRect);
        attempts++;
    }
    
    if (validPos) {
        int powerUpChoice = QRandomGenerator::global()->bounded(3);
        PowerUpType type;
        
        switch (powerUpChoice) {
            case 0: type = PowerUpType::HEALTH; break;
            case 1: type = PowerUpType::BOMB; break;
            case 2: type = PowerUpType::SHIELD; break;
            default: type = PowerUpType::HEALTH;
        }
        
        m_powerUps.push_back(std::make_unique<PowerUp>(pos, type));
    }
}

void GameEngine::triggerBomb() {
    // Destroy all active enemies
    for (auto& enemy : m_enemies) {
        if (enemy->isActive()) {
            enemy->setActive(false);
            m_score += GameConstants::ENEMY_KILL_SCORE;
            m_activeEnemies--;
        }
    }
    emit scoreChanged(m_score);
}

bool GameEngine::isValidMove(const QRectF& rect, Entity* ignore) {
    // Check boundaries
    if (rect.left() < 0 || rect.right() > GameConstants::GAME_AREA_WIDTH ||
        rect.top() < 0 || rect.bottom() > GameConstants::GAME_AREA_HEIGHT) {
        return false;
    }
    
    // Check collisions with blocks
    for (const auto& block : m_blocks) {
        if (!block->isActive() || block.get() == ignore) continue;
        
        Block* blockPtr = static_cast<Block*>(block.get());
        if (blockPtr->blocksMovement() && rect.intersects(block->getRect())) {
            return false;
        }
    }
    
    return true;
}

void GameEngine::cleanupInactive() {
    m_bullets.erase(
        std::remove_if(m_bullets.begin(), m_bullets.end(),
                      [](const auto& bullet) { return !bullet->isActive(); }),
        m_bullets.end()
    );
    
    m_enemies.erase(
        std::remove_if(m_enemies.begin(), m_enemies.end(),
                      [](const auto& enemy) { return !enemy->isActive(); }),
        m_enemies.end()
    );
    
    m_powerUps.erase(
        std::remove_if(m_powerUps.begin(), m_powerUps.end(),
                      [](const auto& powerUp) { return !powerUp->isActive(); }),
        m_powerUps.end()
    );
    
    m_blocks.erase(
        std::remove_if(m_blocks.begin(), m_blocks.end(),
                      [](const auto& block) { return !block->isActive(); }),
        m_blocks.end()
    );
}

void GameEngine::processInput(int key, bool pressed) {
    if (m_state != GameState::PLAYING || !m_player) return;
    
    switch (key) {
        case Qt::Key_W:
        case Qt::Key_Up:
            m_player->setMoving(Direction::UP, pressed);
            break;
        case Qt::Key_S:
        case Qt::Key_Down:
            m_player->setMoving(Direction::DOWN, pressed);
            break;
        case Qt::Key_A:
        case Qt::Key_Left:
            m_player->setMoving(Direction::LEFT, pressed);
            break;
        case Qt::Key_D:
        case Qt::Key_Right:
            m_player->setMoving(Direction::RIGHT, pressed);
            break;
    }
}

void GameEngine::playerShoot() {
    if (m_state != GameState::PLAYING || !m_player || !m_player->canShoot()) return;
    
    QPointF bulletPos = m_player->getRect().center();
    m_bullets.push_back(std::make_unique<Bullet>(bulletPos, 
                                                 m_player->getDirection(), 
                                                 true));
    m_player->resetShootCooldown();
    emit soundEffect("player_shoot");
}

void GameEngine::pauseGame() {
    if (m_state == GameState::PLAYING) {
        m_state = GameState::PAUSED;
        m_gameTimer->stop();
        m_enemySpawnTimer->stop();
        emit gameStateChanged(m_state);
    }
}

void GameEngine::resumeGame() {
    if (m_state == GameState::PAUSED) {
        m_state = GameState::PLAYING;
        m_gameTimer->start();
        m_enemySpawnTimer->start();
        emit gameStateChanged(m_state);
    }
}

void GameEngine::restartGame() {
    startGame();
}

void GameEngine::quitToMenu() {
    m_gameTimer->stop();
    m_enemySpawnTimer->stop();
    m_state = GameState::MENU;
    emit gameStateChanged(m_state);
}
