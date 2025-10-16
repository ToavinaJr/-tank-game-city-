#include "../include/GameEngine.hpp"
#include "../include/Constants.hpp"
#include "../include/GameConfig.hpp"
#include <QRandomGenerator>
#include <algorithm>
#include <QDebug>
#include <QtMath>

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

    qDebug() << "=== Jeu démarré ===";
}

void GameEngine::initializeLevel() {
    m_enemies.clear();
    m_bullets.clear();
    m_blocks.clear();
    m_powerUps.clear();

    m_enemiesRemaining = GameConstants::MAX_ENEMIES;
    m_activeEnemies = 0;

    // IMPORTANT: Créer le niveau AVANT le joueur
    createLevel();

    // Créer le joueur au centre en bas (APRÈS la création du niveau)
    QPointF playerStart(GameConstants::GAME_AREA_WIDTH / 2 - 14,
                        GameConstants::GAME_AREA_HEIGHT - 50);
    m_player = std::make_unique<Tank>(playerStart, EntityType::PLAYER_TANK,
                                      GameConfig::instance().getTankColor(),
                                      GameConstants::PLAYER_SPEED);

    emit playerHealthChanged(m_player->getHealth());

    qDebug() << "Niveau" << m_level << "initialisé";
    qDebug() << "Joueur créé à:" << playerStart;
    qDebug() << "Ennemis à vaincre:" << m_enemiesRemaining;
}

void GameEngine::createLevel() {
    // Position de la base au centre en bas
    QPointF basePos(GameConstants::GAME_AREA_WIDTH / 2 - 16,
                    GameConstants::GAME_AREA_HEIGHT - 64);
    m_blocks.push_back(std::make_unique<Block>(basePos, BlockType::BASE));

    // Position du joueur (pour éviter de placer des blocs ici)
    QPointF playerSpawn(GameConstants::GAME_AREA_WIDTH / 2 - 14,
                        GameConstants::GAME_AREA_HEIGHT - 50);
    QRectF playerArea(playerSpawn.x() - 32, playerSpawn.y() - 32, 96, 96);

    // Créer des murs protecteurs autour de la base (mais pas sur le joueur)
    for (int i = -2; i <= 2; i++) {
        if (i == 0) continue;
        QPointF pos(basePos.x() + i * GameConstants::CELL_SIZE, basePos.y() - 32);

        // Ne pas placer de mur si ça bloque le joueur
        QRectF blockRect(pos, QSizeF(32, 32));
        if (!blockRect.intersects(playerArea)) {
            m_blocks.push_back(std::make_unique<Block>(pos, BlockType::BRICK));
        }
    }

    // Créer un niveau aléatoire
    for (int y = 0; y < GameConstants::GRID_HEIGHT - 5; y++) {  // -5 au lieu de -3 pour plus d'espace en bas
        for (int x = 0; x < GameConstants::GRID_WIDTH; x++) {
            QPointF pos(x * GameConstants::CELL_SIZE, y * GameConstants::CELL_SIZE);
            QRectF blockRect(pos, QSizeF(32, 32));

            // Ne JAMAIS placer de bloc dans la zone de spawn du joueur
            if (blockRect.intersects(playerArea)) {
                continue;
            }

            // Réduire la densité de blocs à 20%
            if (QRandomGenerator::global()->bounded(100) < 20) {
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

    qDebug() << "Niveau créé avec" << m_blocks.size() << "blocs";
    qDebug() << "Zone de spawn joueur protégée:" << playerArea;
}

void GameEngine::update() {
    if (m_state != GameState::PLAYING) return;

    // Sauvegarder la position actuelle du joueur
    QPointF oldPlayerPos = m_player->getPosition();

    // Mettre à jour le joueur (mouvement interne)
    m_player->update();

    // Vérifier si le nouveau mouvement est valide
    QPointF newPlayerPos = m_player->getPosition();

    // Seulement vérifier les collisions si le joueur a bougé
    if (newPlayerPos != oldPlayerPos) {
        if (!isValidMove(m_player->getRect(), m_player.get())) {
            // Restaurer l'ancienne position si collision
            m_player->setPosition(oldPlayerPos);
        }
    }

    // Mettre à jour les ennemis
    updateEnemies();

    // Mettre à jour les balles (SANS vérification de collision ici)
    for (auto& bullet : m_bullets) {
        if (bullet->isActive()) {
            bullet->update();
        }
    }

    // Mettre à jour les power-ups
    for (auto& powerUp : m_powerUps) {
        if (powerUp->isActive()) {
            powerUp->update();
        }
    }

    // Vérifier toutes les collisions APRÈS les mouvements
    checkCollisions();

    // Nettoyer les entités inactives
    cleanupInactive();

    // Vérifier condition de victoire
    if (m_enemiesRemaining == 0 && m_enemies.empty()) {
        m_state = GameState::LEVEL_COMPLETE;
        m_gameTimer->stop();
        m_enemySpawnTimer->stop();
        emit gameStateChanged(m_state);
        qDebug() << "=== NIVEAU TERMINÉ ===";
        qDebug() << "Score final:" << m_score;
    }

    // Vérifier condition de défaite
    if (!m_player->isActive() || m_baseDestroyed) {
        m_state = GameState::GAME_OVER;
        m_gameTimer->stop();
        m_enemySpawnTimer->stop();
        emit gameStateChanged(m_state);
        qDebug() << "=== GAME OVER ===";
        qDebug() << "Score final:" << m_score;
    }
}

void GameEngine::updateEnemies() {
    for (auto& enemy : m_enemies) {
        if (!enemy->isActive()) continue;

        // Sauvegarder la position actuelle
        QPointF oldPos = enemy->getPosition();

        // Mettre à jour (mouvement interne)
        enemy->update();

        // Vérifier si l'ennemi a bougé
        QPointF newPos = enemy->getPosition();

        // Seulement vérifier les collisions si mouvement effectué
        if (newPos != oldPos) {
            if (!isValidMove(enemy->getRect(), enemy.get())) {
                // Restaurer position et forcer changement de direction
                enemy->setPosition(oldPos);
                enemy->updateAI();
            }
        }

        // Tir des ennemis
        if (enemy->shouldShoot() && enemy->canShoot()) {
            QPointF bulletStartPos = enemy->getRect().center();

            // Ajuster la position selon la direction du canon
            Direction dir = enemy->getDirection();
            switch (dir) {
            case Direction::UP:
                bulletStartPos.setY(enemy->getRect().top());
                break;
            case Direction::DOWN:
                bulletStartPos.setY(enemy->getRect().bottom());
                break;
            case Direction::LEFT:
                bulletStartPos.setX(enemy->getRect().left());
                break;
            case Direction::RIGHT:
                bulletStartPos.setX(enemy->getRect().right());
                break;
            }

            m_bullets.push_back(std::make_unique<Bullet>(bulletStartPos, dir, false));
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

        // Vérifier que la position de spawn est valide
        QRectF testRect(spawnPos, QSizeF(28, 28));
        if (isValidMove(testRect)) {
            m_enemies.push_back(std::make_unique<Enemy>(spawnPos));
            m_enemiesRemaining--;
            m_activeEnemies++;
            emit soundEffect("enemy_spawn");
            qDebug() << "Ennemi spawné - Restants:" << m_enemiesRemaining
                     << "Actifs:" << m_activeEnemies;
        }
    }
}

void GameEngine::checkCollisions() {
    checkBulletCollisions();
    checkPowerUpCollisions();
    checkTankCollisions();
}

void GameEngine::checkTankCollisions() {
    // Empêcher le joueur de traverser les ennemis
    if (!m_player->isActive()) return;

    for (auto& enemy : m_enemies) {
        if (!enemy->isActive()) continue;

        if (m_player->collidesWith(*enemy)) {
            // Calculer la direction de répulsion
            QPointF playerPos = m_player->getPosition();
            QPointF enemyPos = enemy->getPosition();

            QPointF direction = playerPos - enemyPos;
            qreal length = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());

            if (length > 0) {
                direction /= length;
                m_player->setPosition(playerPos + direction * 3);
            }
        }
    }
}

void GameEngine::checkBulletCollisions() {
    // CORRECTION MAJEURE: Vérifier TOUTES les collisions de balles
    for (auto& bullet : m_bullets) {
        if (!bullet->isActive()) continue;

        bool bulletHit = false;

        // 1. Collision balle vs blocs
        for (auto& block : m_blocks) {
            if (!block->isActive() || bulletHit) continue;

            if (bullet->collidesWith(*block)) {
                Block* blockPtr = static_cast<Block*>(block.get());

                // Si c'est la base, game over
                if (blockPtr->getBlockType() == BlockType::BASE) {
                    m_baseDestroyed = true;
                    emit soundEffect("base_destroyed");
                    qDebug() << "!!! BASE DÉTRUITE !!!";
                }

                // Si destructible, détruire le bloc
                if (blockPtr->isDestructible()) {
                    block->setActive(false);
                    emit soundEffect("block_destroyed");
                }

                // Les arbres ne bloquent pas les balles
                if (!blockPtr->isCamouflage()) {
                    bullet->setActive(false);
                    bulletHit = true;
                }
            }
        }

        if (bulletHit) continue;

        // 2. CORRECTION: Collision balles du joueur vs ennemis
        if (bullet->isFromPlayer()) {
            for (auto& enemy : m_enemies) {
                if (!enemy->isActive() || bulletHit) continue;

                if (bullet->collidesWith(*enemy)) {
                    qDebug() << ">>> Balle du joueur touche un ennemi!";

                    enemy->takeDamage(1);
                    bullet->setActive(false);
                    bulletHit = true;

                    if (!enemy->isActive()) {
                        // Ennemi détruit
                        m_score += GameConstants::ENEMY_KILL_SCORE;
                        m_activeEnemies--;
                        emit scoreChanged(m_score);
                        emit soundEffect("enemy_destroyed");

                        qDebug() << "*** ENNEMI DÉTRUIT ***";
                        qDebug() << "Score:" << m_score;
                        qDebug() << "Ennemis actifs restants:" << m_activeEnemies;
                        qDebug() << "Ennemis à spawner:" << m_enemiesRemaining;

                        // Chance de drop power-up (30%)
                        if (QRandomGenerator::global()->bounded(100) < 30) {
                            spawnPowerUp(enemy->getPosition());
                        }
                    }
                    break;
                }
            }
        }
        // 3. Collision balles ennemies vs joueur
        else {
            if (m_player->isActive() && !bulletHit) {
                if (bullet->collidesWith(*m_player)) {
                    qDebug() << "<<< Balle ennemie touche le joueur!";

                    m_player->takeDamage(1);
                    bullet->setActive(false);
                    bulletHit = true;

                    emit playerHealthChanged(m_player->getHealth());
                    emit soundEffect("player_hit");

                    qDebug() << "Santé joueur:" << m_player->getHealth();
                }
            }
        }
    }

    // 4. Collision balle vs balle
    for (size_t i = 0; i < m_bullets.size(); i++) {
        if (!m_bullets[i]->isActive()) continue;

        for (size_t j = i + 1; j < m_bullets.size(); j++) {
            if (!m_bullets[j]->isActive()) continue;

            if (m_bullets[i]->collidesWith(*m_bullets[j])) {
                m_bullets[i]->setActive(false);
                m_bullets[j]->setActive(false);
                qDebug() << "Collision balle vs balle";
            }
        }
    }
}

void GameEngine::checkPowerUpCollisions() {
    if (!m_player->isActive()) return;

    for (auto& powerUp : m_powerUps) {
        if (!powerUp->isActive()) continue;

        if (powerUp->collidesWith(*m_player)) {
            PowerUp* powerUpPtr = static_cast<PowerUp*>(powerUp.get());

            switch (powerUpPtr->getPowerUpType()) {
            case PowerUpType::HEALTH:
                m_player->heal(1);
                emit playerHealthChanged(m_player->getHealth());
                emit soundEffect("powerup_health");
                qDebug() << "❤️ Power-up SANTÉ collecté - Santé:" << m_player->getHealth();
                break;

            case PowerUpType::BOMB:
                triggerBomb();
                emit soundEffect("powerup_bomb");
                qDebug() << "💣 BOMBE activée!";
                break;

            case PowerUpType::SHIELD:
                m_player->activateShield(300);
                emit soundEffect("powerup_shield");
                qDebug() << "🛡️ BOUCLIER activé";
                break;
            }

            powerUp->setActive(false);
            m_score += 50;
            emit scoreChanged(m_score);
        }
    }
}

void GameEngine::spawnPowerUp(const QPointF& position) {
    QPointF pos = position;

    // Si pas de position fournie, chercher une position aléatoire valide
    if (pos.isNull()) {
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

        if (!validPos) {
            qDebug() << "Impossible de trouver position valide pour power-up";
            return;
        }
    }

    // Choisir un type de power-up aléatoire
    int powerUpChoice = QRandomGenerator::global()->bounded(3);
    PowerUpType type;

    switch (powerUpChoice) {
    case 0: type = PowerUpType::HEALTH; break;
    case 1: type = PowerUpType::BOMB; break;
    case 2: type = PowerUpType::SHIELD; break;
    default: type = PowerUpType::HEALTH;
    }

    m_powerUps.push_back(std::make_unique<PowerUp>(pos, type));
    qDebug() << "✨ Power-up spawné à" << pos;
}

void GameEngine::triggerBomb() {
    // Détruire tous les ennemis actifs
    int destroyed = 0;
    for (auto& enemy : m_enemies) {
        if (enemy->isActive()) {
            enemy->setActive(false);
            m_score += GameConstants::ENEMY_KILL_SCORE;
            m_activeEnemies--;
            destroyed++;
        }
    }

    if (destroyed > 0) {
        emit scoreChanged(m_score);
        qDebug() << "💥 BOMBE:" << destroyed << "ennemis détruits! Score:" << m_score;
    }
}

bool GameEngine::isValidMove(const QRectF& rect, Entity* ignore) {
    // Vérifier les limites du terrain
    if (rect.left() < 0 || rect.right() > GameConstants::GAME_AREA_WIDTH ||
        rect.top() < 0 || rect.bottom() > GameConstants::GAME_AREA_HEIGHT) {
        return false;
    }

    // Vérifier les collisions avec les blocs
    for (const auto& block : m_blocks) {
        if (!block->isActive() || block.get() == ignore) continue;

        Block* blockPtr = static_cast<Block*>(block.get());

        // Les arbres ne bloquent pas le mouvement
        if (blockPtr->blocksMovement()) {
            // Vérifier intersection avec une petite marge
            if (rect.intersects(block->getRect())) {
                // Si c'est le joueur au spawn initial, autoriser quand même
                if (ignore == m_player.get()) {
                    QPointF playerSpawn(GameConstants::GAME_AREA_WIDTH / 2 - 14,
                                        GameConstants::GAME_AREA_HEIGHT - 50);
                    QRectF spawnArea(playerSpawn.x() - 5, playerSpawn.y() - 5, 38, 38);

                    // Si on est proche du spawn, autoriser
                    if (spawnArea.intersects(rect)) {
                        continue;
                    }
                }
                return false;
            }
        }
    }

    // Vérifier collision avec le joueur
    if (m_player.get() != ignore && m_player->isActive()) {
        if (rect.intersects(m_player->getRect())) {
            return false;
        }
    }

    // Vérifier collision avec les autres ennemis
    for (const auto& enemy : m_enemies) {
        if (enemy.get() != ignore && enemy->isActive()) {
            if (rect.intersects(enemy->getRect())) {
                return false;
            }
        }
    }

    return true;
}

void GameEngine::cleanupInactive() {
    // Nettoyer les balles inactives
    size_t bulletsBefore = m_bullets.size();
    m_bullets.erase(
        std::remove_if(m_bullets.begin(), m_bullets.end(),
                       [](const auto& bullet) { return !bullet->isActive(); }),
        m_bullets.end()
        );

    if (bulletsBefore != m_bullets.size()) {
        qDebug() << "🧹 Balles nettoyées:" << (bulletsBefore - m_bullets.size())
                 << "| Restantes:" << m_bullets.size();
    }

    // Nettoyer les ennemis inactifs
    size_t enemiesBefore = m_enemies.size();
    m_enemies.erase(
        std::remove_if(m_enemies.begin(), m_enemies.end(),
                       [](const auto& enemy) { return !enemy->isActive(); }),
        m_enemies.end()
        );

    if (enemiesBefore != m_enemies.size()) {
        qDebug() << "🧹 Ennemis nettoyés:" << (enemiesBefore - m_enemies.size());
    }

    // Nettoyer les power-ups inactifs
    m_powerUps.erase(
        std::remove_if(m_powerUps.begin(), m_powerUps.end(),
                       [](const auto& powerUp) { return !powerUp->isActive(); }),
        m_powerUps.end()
        );

    // Nettoyer les blocs inactifs
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
    if (m_state != GameState::PLAYING || !m_player || !m_player->canShoot()) {
        return;
    }

    // Calculer la position de départ de la balle selon la direction du canon
    QPointF bulletStartPos = m_player->getRect().center();
    Direction dir = m_player->getDirection();

    switch (dir) {
    case Direction::UP:
        bulletStartPos.setY(m_player->getRect().top());
        break;
    case Direction::DOWN:
        bulletStartPos.setY(m_player->getRect().bottom());
        break;
    case Direction::LEFT:
        bulletStartPos.setX(m_player->getRect().left());
        break;
    case Direction::RIGHT:
        bulletStartPos.setX(m_player->getRect().right());
        break;
    }

    m_bullets.push_back(std::make_unique<Bullet>(bulletStartPos, dir, true));
    m_player->resetShootCooldown();
    emit soundEffect("player_shoot");

    qDebug() << "🔫 Joueur tire - Direction:" << static_cast<int>(dir)
             << "| Balles totales:" << m_bullets.size();
}

void GameEngine::pauseGame() {
    if (m_state == GameState::PLAYING) {
        m_state = GameState::PAUSED;
        m_gameTimer->stop();
        m_enemySpawnTimer->stop();
        emit gameStateChanged(m_state);
        qDebug() << "⏸️ Jeu en pause";
    }
}

void GameEngine::resumeGame() {
    if (m_state == GameState::PAUSED) {
        m_state = GameState::PLAYING;
        m_gameTimer->start();
        m_enemySpawnTimer->start();
        emit gameStateChanged(m_state);
        qDebug() << "▶️ Jeu repris";
    }
}

void GameEngine::restartGame() {
    qDebug() << "🔄 Redémarrage du jeu";
    startGame();
}

void GameEngine::quitToMenu() {
    m_gameTimer->stop();
    m_enemySpawnTimer->stop();
    m_state = GameState::MENU;
    emit gameStateChanged(m_state);
    qDebug() << "🏠 Retour au menu";
}
