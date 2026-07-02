#include "./../headers/LinuxAPI.h"

std::string LinuxAPI::getSystemName() {
   return "Linux";
}



// ------------------------------------- Чтение системного CPU -------------------------------------
void LinuxAPI::updateSystemCpu() {
   ifstream stat("/proc/stat");
   if (!stat.is_open()) {
      cout << "Couldn't open the statistics file" << endl;
      return;
   }
   string line;
   while (getline(stat, line)) {
      if (line.compare(0, 4, "cpu ") == 0) {
         istringstream iss(line.substr(4));
         CpuStats cur;
         iss >> cur.user >> cur.nice >> cur.system >> cur.idle >> cur.iowait >> cur.irq >> cur.softirq >> cur.steal;

         if (!firstSysCpu) {
            unsigned long long prevTotal = prevSysCpu.user + prevSysCpu.nice +
                                          prevSysCpu.system + prevSysCpu.idle +
                                          prevSysCpu.iowait + prevSysCpu.irq +
                                          prevSysCpu.softirq + prevSysCpu.steal;
            unsigned long long curTotal = cur.user + cur.nice + cur.system + cur.idle +
                                          cur.iowait + cur.irq + cur.softirq + cur.steal;
            unsigned long long prevIdle = prevSysCpu.idle + prevSysCpu.iowait;
            unsigned long long curIdle = cur.idle + cur.iowait;

            unsigned long long diffTotal = curTotal - prevTotal;
            unsigned long long diffIdle = curIdle - prevIdle;
            if (diffTotal > 0) {
               SystemCpuUsagePercent = 100.0 * (diffTotal - diffIdle) / diffTotal;
            }
         }
         prevSysCpu = cur;
         firstSysCpu = false;
      }
   }

   stat.close();
   return;
}

void LinuxAPI::updateAppCpu() {
   return;
}

// ----------------------------------------- Память системы ----------------------------------------
void LinuxAPI::updateSystemMemory() {
   ifstream meminfo("/proc/meminfo");
   if (!meminfo.is_open()) {
      cout << "Couldn't open the statistics file" << endl;
      return;
   }
   string line;
   unsigned long long memTotal = 0, memAvailable = 0;
   while (getline(meminfo, line)) {
      if (line.compare(0, 9, "MemTotal:") == 0) {
            istringstream iss(line);
            string key;
            iss >> key >> memTotal;
      } else if (line.compare(0, 13, "MemAvailable:") == 0) {
            std::istringstream iss(line);
            std::string key;
            iss >> key >> memAvailable;
            break;
      }
   }
   if (memTotal > 0) {
      SystemMemoryUsagePercent = 100.0 * (memTotal - memAvailable) / memTotal;
   }
   return;
}

void LinuxAPI::updateAppMemory() {
   return;
}

void LinuxAPI::updateTasksCount() {
   return;
}

void LinuxAPI::updateThreadsCount() {
   return;
}

void LinuxAPI::updateDiskStats() {
   return;
}

// ----------------------------------------- Задержка в мкс ----------------------------------------
void LinuxAPI::usDelay(int delay) {
   usleep(delay);
   return;
}