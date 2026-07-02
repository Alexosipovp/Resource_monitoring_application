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
        cout << api->getSystemMemoryUsagePercent() << endl;

        api->usDelay(1000000);
    }

    cout << "Running on: " << api->getSystemName() << endl;
    return 0;
}