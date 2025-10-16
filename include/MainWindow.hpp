#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// Utilisez le mot-clé Q_OBJECT si vous avez des slots/signals
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT // Macro nécessaire pour les fonctionnalités MOC (slots/signals/propriétés)

public:
    // Constructeur : le parent est nullptr pour la fenêtre de niveau supérieur
    MainWindow(QWidget *parent = nullptr){}
    // Destructeur
    ~MainWindow(){}

private:
    // Pointeur pour gérer l'UI générée par Qt Designer (.ui)
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H