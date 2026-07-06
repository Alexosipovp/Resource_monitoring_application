#include "./../headers/PlatformAPI.h"

void PlatformAPI::followApp(string nameApp) {
    this->nameApp = nameApp;
    return;
}

void PlatformAPI::enableMonitoring(bool enable) {
    monitoring = enable;
    return;
}

double PlatformAPI::getSystemCpuUsagePercent() {
    return SystemCpuUsagePercent;
}

double PlatformAPI::getAppCpuUsagePercent() {
    return AppCpuUsagePercent;
}

double PlatformAPI::getSystemMemoryUsagePercent() {
    return SystemMemoryUsagePercent;
}

unsigned long long PlatformAPI::getSystemMemoryAvailableMB() {
    return SystemMemoryAvailableMB;
}

unsigned long long PlatformAPI::getAppMemoryUsageMB() {
    return AppMemoryUsageMB;
}

unsigned int PlatformAPI::getRunningTasksCount() {
    return RunningTasksCount;
}

unsigned int PlatformAPI::getAppThreadsCount() {
    return AppThreadsCount;
}

double PlatformAPI::getDiskReadSpeedMBps() {
    return DiskReadSpeedMBps;
}

double PlatformAPI::getDiskWriteSpeedMBps() {
    return DiskWriteSpeedMBps;
}

void PlatformAPI::updateData() {
    updateSystemCpu();
    updateAppCpu();
    updateSystemMemory();
    updateAppMemory();
    updateTasksCount();
    updateThreadsCount();
    updateDiskStats();
}