#include <memory>

#include "headers/PlatformFactory.h"

int main(int argc, char *argv[])
{
    std::unique_ptr<PlatformAPI> api = PlatformFactory::create();
    if (!api) {
        cout << "Unsupported platform!" << endl;
        return 1;
    }

    while (true) {
        api->updateData();
        cout << api->getDiskWriteSpeedMBps() << endl;

        api->usDelay(1000000);
    }

    cout << "Running on: " << api->getRunningTasksCount() << endl;
    return 0;
}