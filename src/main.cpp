#include <memory>

#include "headers/PlatformFactory.h"

int main(int argc, char *argv[])
{
    std::unique_ptr<IPlatformAPI> api = PlatformFactory::create();
    if (!api) {
        cout << "Unsupported platform!" << endl;
        return 1;
    }

    cout << "Running on: " << api->getSystemName() << endl;
    return 0;
}