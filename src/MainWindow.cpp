#include "../include/GameWidget.hpp"
#include "../include/GameEngine.hpp"

#include "../include/MainWindow.hpp"
#include "../include/GameEngine.hpp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(800, 600);

    // --- Menu ---
    m_menu = new MenuWidget(this);

    // --- Moteur du jeu ---
    m_gameEngine = new GameEngine(this);

    // --- Widget de jeu ---
    m_gameScene = new GameWidget(m_gameEngine, this);

    // --- QStackedWidget pour basculer menu / jeu ---
    m_stack = new QStackedWidget(this);
    m_stack->addWidget(m_menu);
    m_stack->addWidget(m_gameScene);

    setCentralWidget(m_stack);

    // --- Connexions menu ---
    connect(m_menu, &MenuWidget::startGame, this, &MainWindow::onStartGame);
    connect(m_menu, &MenuWidget::openSettings, this, &MainWindow::onOpenSettings);
    connect(m_menu, &MenuWidget::quitGame, this, &MainWindow::close);

    // --- Connexions GameEngine
    connect(m_gameEngine, &GameEngine::gameStateChanged, this, [&](GameState state){
        if(state == GameState::GAME_OVER){
            m_stack->setCurrentWidget(m_menu);
        }
    });

    // Démarrer l’update du moteur
    m_gameEngine->startGame();
}

void MainWindow::onStartGame()
{
    m_stack->setCurrentWidget(m_gameScene);
    m_gameEngine->restartGame();
    m_gameScene->setFocus();
    qDebug() << "Le jeu démarre !";
}

void MainWindow::onOpenSettings()
{
    qDebug() << "Ouvrir la fenêtre des paramètres";
}
