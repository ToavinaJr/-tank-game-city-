#include "../include/GameWidget.hpp"
#include "../include/Constants.hpp"
#include <QPainter>
#include <QFont>

GameWidget::GameWidget(GameEngine* engine, QWidget* parent)
    : QWidget(parent)
    , m_engine(engine)
{
    setFixedSize(GameConstants::GAME_AREA_WIDTH, GameConstants::GAME_AREA_HEIGHT);
    setFocusPolicy(Qt::StrongFocus);
}

void GameWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw background
    painter.fillRect(rect(), QColor(Colors::BACKGROUND));
    
    if (m_engine->getState() == GameState::PLAYING) {
        renderGame(painter);
    } else if (m_engine->getState() == GameState::PAUSED) {
        renderGame(painter);
        renderPauseScreen(painter);
    } else if (m_engine->getState() == GameState::GAME_OVER) {
        renderGame(painter);
        renderGameOverScreen(painter);
    } else if (m_engine->getState() == GameState::LEVEL_COMPLETE) {
        renderGame(painter);
        renderLevelCompleteScreen(painter);
    }
}

void GameWidget::renderGame(QPainter& painter) {
    renderBlocks(painter);
    renderPowerUps(painter);
    renderBullets(painter);
    renderTanks(painter);
}

void GameWidget::renderBlocks(QPainter& painter) {
    for (const auto& block : m_engine->getBlocks()) {
        if (block->isActive()) {
            block->render(painter);
        }
    }
}

void GameWidget::renderTanks(QPainter& painter) {
    // Render enemies
    for (const auto& enemy : m_engine->getEnemies()) {
        if (enemy->isActive()) {
            enemy->render(painter);
        }
    }
    
    // Render player on top
    if (m_engine->getPlayer() && m_engine->getPlayer()->isActive()) {
        m_engine->getPlayer()->render(painter);
    }
}

void GameWidget::renderBullets(QPainter& painter) {
    for (const auto& bullet : m_engine->getBullets()) {
        if (bullet->isActive()) {
            bullet->render(painter);
        }
    }
}

void GameWidget::renderPowerUps(QPainter& painter) {
    for (const auto& powerUp : m_engine->getPowerUps()) {
        if (powerUp->isActive()) {
            powerUp->render(painter);
        }
    }
}

void GameWidget::renderPauseScreen(QPainter& painter) {
    painter.save();
    
    // Semi-transparent overlay
    painter.fillRect(rect(), QColor(0, 0, 0, 180));
    
    // Pause text
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPixelSize(48);
    font.setBold(true);
    painter.setFont(font);
    
    painter.drawText(rect(), Qt::AlignCenter, "PAUSED");
    
    font.setPixelSize(20);
    font.setBold(false);
    painter.setFont(font);
    
    QRect textRect = rect().adjusted(0, 80, 0, 0);
    painter.drawText(textRect, Qt::AlignCenter, "Press ESC to resume\nPress Q to quit");
    
    painter.restore();
}

void GameWidget::renderGameOverScreen(QPainter& painter) {
    painter.save();
    
    painter.fillRect(rect(), QColor(0, 0, 0, 200));
    
    painter.setPen(QColor(255, 50, 50));
    QFont font = painter.font();
    font.setPixelSize(56);
    font.setBold(true);
    painter.setFont(font);
    
    painter.drawText(rect(), Qt::AlignCenter, "GAME OVER");
    
    font.setPixelSize(24);
    font.setBold(false);
    painter.setFont(font);
    painter.setPen(Qt::white);
    
    QRect textRect = rect().adjusted(0, 100, 0, 0);
    QString scoreText = QString("Score: %1").arg(m_engine->getScore());
    painter.drawText(textRect, Qt::AlignCenter, scoreText);
    
    textRect = rect().adjusted(0, 150, 0, 0);
    painter.drawText(textRect, Qt::AlignCenter, "Press R to restart\nPress Q to quit");
    
    painter.restore();
}

void GameWidget::renderLevelCompleteScreen(QPainter& painter) {
    painter.save();
    
    painter.fillRect(rect(), QColor(0, 0, 0, 180));
    
    painter.setPen(QColor(50, 255, 50));
    QFont font = painter.font();
    font.setPixelSize(48);
    font.setBold(true);
    painter.setFont(font);
    
    painter.drawText(rect(), Qt::AlignCenter, "LEVEL COMPLETE!");
    
    font.setPixelSize(24);
    font.setBold(false);
    painter.setFont(font);
    painter.setPen(Qt::white);
    
    QRect textRect = rect().adjusted(0, 100, 0, 0);
    QString scoreText = QString("Score: %1").arg(m_engine->getScore());
    painter.drawText(textRect, Qt::AlignCenter, scoreText);
    
    textRect = rect().adjusted(0, 150, 0, 0);
    painter.drawText(textRect, Qt::AlignCenter, "Press SPACE to continue\nPress Q to quit");
    
    painter.restore();
}

void GameWidget::keyPressEvent(QKeyEvent* event) {
    if (event->isAutoRepeat()) {
        event->ignore();
        return;
    }
    
    switch (event->key()) {
        case Qt::Key_Space:
            if (m_engine->getState() == GameState::PLAYING) {
                m_engine->playerShoot();
            } else if (m_engine->getState() == GameState::LEVEL_COMPLETE) {
                m_engine->restartGame();
            }
            break;
            
        case Qt::Key_Escape:
            if (m_engine->getState() == GameState::PLAYING) {
                m_engine->pauseGame();
            } else if (m_engine->getState() == GameState::PAUSED) {
                m_engine->resumeGame();
            }
            break;
            
        case Qt::Key_R:
            if (m_engine->getState() == GameState::GAME_OVER) {
                m_engine->restartGame();
            }
            break;
            
        case Qt::Key_Q:
            if (m_engine->getState() != GameState::PLAYING) {
                m_engine->quitToMenu();
            }
            break;
            
        default:
            m_engine->processInput(event->key(), true);
            break;
    }
    
    update();
}

void GameWidget::keyReleaseEvent(QKeyEvent* event) {
    if (event->isAutoRepeat()) {
        event->ignore();
        return;
    }
    
    m_engine->processInput(event->key(), false);
    update();
}
