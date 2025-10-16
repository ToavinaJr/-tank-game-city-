#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>
#include <QTimer>
#include <memory>
#include <vector>
#include "Tank.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"
#include "Block.hpp"
#include "PowerUp.hpp"

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    LEVEL_COMPLETE
};

class GameEngine : public QObject {
    Q_OBJECT

public:
    explicit GameEngine(QObject* parent = nullptr);
    ~GameEngine();

    void startGame();
    void pauseGame();
    void resumeGame();
    void restartGame();
    void quitToMenu();

    void processInput(int key, bool pressed);
    void playerShoot();

    GameState getState() const { return m_state; }
    Tank* getPlayer() const { return m_player.get(); }
    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const { return m_enemies; }
    const std::vector<std::unique_ptr<Bullet>>& getBullets() const { return m_bullets; }
    const std::vector<std::unique_ptr<Block>>& getBlocks() const { return m_blocks; }
    const std::vector<std::unique_ptr<PowerUp>>& getPowerUps() const { return m_powerUps; }

    int getScore() const { return m_score; }
    int getLevel() const { return m_level; }
    int getEnemiesRemaining() const { return m_enemiesRemaining; }

signals:
    void gameStateChanged(GameState state);
    void scoreChanged(int score);
    void playerHealthChanged(int health);
    void levelChanged(int level);
    void soundEffect(const QString& effect);

private slots:
    void update();
    void spawnEnemy();

private:
    void initializeLevel();
    void createLevel();
    void checkCollisions();
    void checkBulletCollisions();
    void checkPowerUpCollisions();
    void checkTankCollisions();  // NOUVEAU - collision tank-tank
    void updateEnemies();
    void cleanupInactive();
    void spawnPowerUp(const QPointF& position = QPointF());  // Position optionnelle
    void triggerBomb();

    bool isValidMove(const QRectF& rect, Entity* ignore = nullptr);
    QPointF getSpawnPosition(int index);

    GameState m_state;
    std::unique_ptr<Tank> m_player;
    std::vector<std::unique_ptr<Enemy>> m_enemies;
    std::vector<std::unique_ptr<Bullet>> m_bullets;
    std::vector<std::unique_ptr<Block>> m_blocks;
    std::vector<std::unique_ptr<PowerUp>> m_powerUps;

    QTimer* m_gameTimer;
    QTimer* m_enemySpawnTimer;

    int m_score;
    int m_level;
    int m_enemiesRemaining;
    int m_activeEnemies;
    bool m_baseDestroyed;
};

#endif // GAMEENGINE_H
