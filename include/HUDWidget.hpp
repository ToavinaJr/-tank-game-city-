#ifndef HUDWIDGET_H
#define HUDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QProgressBar>

class HUDWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit HUDWidget(QWidget* parent = nullptr);
    
public slots:
    void updateHealth(int health);
    void updateScore(int score);
    void updateLevel(int level);
    void updateEnemiesRemaining(int enemies);
    
private:
    void setupUI();
    
    QLabel* m_healthLabel;
    QLabel* m_scoreLabel;
    QLabel* m_levelLabel;
    QLabel* m_enemiesLabel;
    QProgressBar* m_healthBar;
};

#endif // HUDWIDGET_H
