#include <QtWidgets/QApplication>
#include "./headers/mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    SystemMonitorWindow window;
    window.show();
    
    return app.exec();
}