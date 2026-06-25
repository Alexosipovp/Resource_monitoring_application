#include <iostream>
#include "./headers/test.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MyWindow window;
    window.setWindowTitle("Простое Qt-приложение");
    window.resize(300, 150);
    window.show();

    return app.exec();
}