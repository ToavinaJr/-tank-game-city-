#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QWidget>
#include <QRectF>

class GameScene : public QWidget {
    Q_OBJECT

public:
    explicit GameScene(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        // Exemple : définir une taille initiale
        setFixedSize(800, 600);
    }

    QRectF getSceneBounds() const { return m_sceneBounds; }
    void setSceneBounds(const QRectF& bounds) { m_sceneBounds = bounds; }

protected:
    // Tu pourras redéfinir paintEvent pour dessiner le jeu
    void paintEvent(QPaintEvent* event) override {
        QWidget::paintEvent(event);
        // Dessin du jeu ici
    }

private:
    QRectF m_sceneBounds;
};

#endif // GAMESCENE_H
