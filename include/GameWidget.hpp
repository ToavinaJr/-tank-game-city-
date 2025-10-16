#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include "GameEngine.hpp"

class GameWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit GameWidget(GameEngine* engine, QWidget* parent = nullptr);
    
protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    
private:
    void renderGame(QPainter& painter);
    void renderBlocks(QPainter& painter);
    void renderTanks(QPainter& painter);
    void renderBullets(QPainter& painter);
    void renderPowerUps(QPainter& painter);
    void renderPauseScreen(QPainter& painter);
    void renderGameOverScreen(QPainter& painter);
    void renderLevelCompleteScreen(QPainter& painter);
    
    GameEngine* m_engine;
};

#endif // GAMEWIDGET_H
