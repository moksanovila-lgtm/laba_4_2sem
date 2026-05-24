#include <QApplication>
#include <QStyleFactory>
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    app.setApplicationName("Sequence System");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("University");
    
    app.setStyle(QStyleFactory::create("Fusion"));
    
    MainWindow window;
    window.setWindowTitle("Sequence System - Assignment 4");
    window.setMinimumSize(900, 700);
    window.show();
    
    return app.exec();
}