#include <iostream>
#include <QApplication>
#include <QLabel>

#include "headers/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLabel label;
    label.setText("Hello world " + QString::number(test_func()));
    label.show();
    return app.exec();
}