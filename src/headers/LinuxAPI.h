#ifndef LINUXAPI_H
#define LINUXAPI_H

#include <iostream>

#include "IPlatformAPI.h"

class LinuxAPI : public IPlatformAPI {
public:
    ~LinuxAPI(){};
    std::string getSystemName();
};

#endif