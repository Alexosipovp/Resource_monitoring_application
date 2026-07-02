#ifndef PLATFORMAPI_H
#define PLATFORMAPI_H

#include <iostream>
using namespace std;

class PlatformAPI {
public:
    virtual ~PlatformAPI() {};

    virtual std::string getSystemName() = 0;
    void PIDMonitor(int);
    void enableMonitoring(bool);

    double getSystemCpuUsagePercent();
    double getAppCpuUsagePercent();
    double getSystemMemoryUsagePercent();
    unsigned long long getSystemMemoryAvailableMB();
    unsigned long long getAppMemoryUsageMB();
    unsigned int getRunningTasksCount();
    unsigned int getAppThreadsCount();
    double getDiskReadSpeedMBps();
    double getDiskWriteSpeedMBps();

    virtual void updateData();
    virtual void updateSystemCpu() = 0;
    virtual void updateAppCpu() = 0;
    virtual void updateSystemMemory() = 0;
    virtual void updateAppMemory() = 0;
    virtual void updateTasksCount() = 0;
    virtual void updateThreadsCount() = 0;
    virtual void updateDiskStats() = 0;
    virtual void usDelay(int) = 0;

protected:
    int pid = -1;
    bool monitoring = false;
    double SystemCpuUsagePercent = 0;
    double AppCpuUsagePercent = 0;
    double SystemMemoryUsagePercent = 0;
    unsigned long long SystemMemoryAvailableMB = 0;
    unsigned long long AppMemoryUsageMB = 0;
    unsigned int RunningTasksCount = 0;
    unsigned int AppThreadsCount = 0;
    double DiskReadSpeedMBps = 0;
    double DiskWriteSpeedMBps = 0;
};

#endif