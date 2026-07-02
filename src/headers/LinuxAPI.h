#ifndef LINUXAPI_H
#define LINUXAPI_H

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>

#include "PlatformAPI.h"


class LinuxAPI : public PlatformAPI {
public:
    ~LinuxAPI(){};
    std::string getSystemName();

    void updateSystemCpu();
    void updateAppCpu();
    void updateSystemMemory();
    void updateAppMemory();
    void updateTasksCount();
    void updateThreadsCount();
    void updateDiskStats();
    void usDelay(int);
private:
    struct CpuStats {
        unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
    };

    struct ProcCpuStats {
        unsigned long long utime, stime, cutime, cstime, starttime;
    };

    bool firstSysCpu = true;
    CpuStats prevSysCpu; 
};

#endif