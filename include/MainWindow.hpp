#include "MenuWidget.hpp"
#include "GameWidget.hpp"
#include "GameEngine.hpp"
#include <QMainWindow>
#include <QStackedWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private slots:
    void onStartGame();      // SLOT pour démarrer le jeu
    void onOpenSettings();
private:
    QStackedWidget* m_stack = nullptr;
    MenuWidget* m_menu = nullptr;
    GameWidget* m_gameScene = nullptr; // Widget de jeu réel
    GameEngine* m_gameEngine = nullptr; // Logique du jeu
};
