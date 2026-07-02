#ifndef IPLATFORMAPI_H
#define IPLATFORMAPI_H

#include <iostream>
using namespace std;

class IPlatformAPI {
public:
    virtual ~IPlatformAPI() {};
    virtual std::string getSystemName() = 0;
};

#endif