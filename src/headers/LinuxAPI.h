#ifndef LINUXAPI_H
#define LINUXAPI_H

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <vector>

#include "PlatformAPI.h"


class LinuxAPI : public PlatformAPI {
public:
    ~LinuxAPI(){};
    string getSystemName();

    void updateData();
    void updateSystemCpu();
    void updateAppCpu();
    void updateSystemMemory();
    void updateAppMemory();
    void updateTasksCount();
    void updateThreadsCount();
    void updateDiskStats();
    void usDelay(int);
    void updatePid();
private:
    struct CpuStats {
        unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
    };

    struct ProcCpuStats {
        unsigned long long utime, stime, cutime, cstime, starttime;
    };

    int pid = 0;
    bool firstSysCpu = true;
    bool firstProcCpu = true;
    bool diskDeviceFound = false;
    string diskDevice;
    CpuStats prevSysCpu; 
    ProcCpuStats prevProcCpu;
    double diskReadMBps;
    double diskWriteMBps;
    bool firstDisk = true;
    unsigned long long prevWriteSectors = 0;
    unsigned long long prevReadSectors = 0;
    unsigned long long prevSysTotal = 0;
};

#endif