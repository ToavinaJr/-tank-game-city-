#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QObject>
#include <QRectF>

class GameScene : public QObject {
    Q_OBJECT
    
public:
    explicit GameScene(QObject* parent = nullptr);
    
    QRectF getSceneBounds() const { return m_sceneBounds; }
    void setSceneBounds(const QRectF& bounds) { m_sceneBounds = bounds; }
    
private:
    QRectF m_sceneBounds;
};

#endif // GAMESCENE_H