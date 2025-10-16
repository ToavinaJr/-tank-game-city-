#include <QApplication>
#include "../include/MainWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application information
    QApplication::setApplicationName("Tank Battle Game");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("TankBattleGame");
    QApplication::setOrganizationDomain("tankbattle.com");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}