#include <QtWidgets/QApplication>
#include <memory>

#include "headers/mainwindow.h"
#include "headers/PlatformFactory.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    SystemMonitorWindow window;
    window.show();
    
    return app.exec();

    /*
    std::unique_ptr<PlatformAPI> api = PlatformFactory::create();
    if (!api) {
        cout << "Unsupported platform!" << endl;
        return 1;
    }

    api->followApp("firefox");

    while (true) {
        api->updateData();
        cout << api->getAppThreadsCount() << endl;

        api->usDelay(1000000);
    }

    cout << "Running on: " << api->getRunningTasksCount() << endl;
    return 0;
    */
}