#ifndef PLATFORM_FACTORY_H
#define PLATFORM_FACTORY_H

#include "LinuxAPI.h"

class PlatformFactory {
public:
    static std::unique_ptr<PlatformAPI> create() {
        #ifdef _WIN32
            return std::unique_ptr<WindowsAPI>(new LinuxAPI());
        #elif defined(__linux__)
            return std::unique_ptr<LinuxAPI>(new LinuxAPI());
        #else
            return nullptr;
        #endif
    }
};

#endif