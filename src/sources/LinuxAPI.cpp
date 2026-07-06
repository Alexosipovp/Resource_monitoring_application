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
   meminfo.close();
   return;
}

void LinuxAPI::updateAppMemory() {
   return;
}

// ---------------------------------- Количество запущенных задач ----------------------------------
void LinuxAPI::updateTasksCount() {
   DIR* dir = opendir("/proc");
   if (!dir) {
      RunningTasksCount = 0;
      return;
   }
   unsigned int count = 0;
   struct dirent* entry;
   while ((entry = readdir(dir)) != nullptr) {
      if (entry->d_type == DT_DIR) {
            char* endptr;
            long pid = strtol(entry->d_name, &endptr, 10);
            if (*endptr == '\0' && pid > 0) {
               count++;
            }
      }
   }
   closedir(dir);
   RunningTasksCount = count;
   return;
}

void LinuxAPI::updateThreadsCount() {
   return;
}

// --------------------------------- Скорость чтения/записи диска ----------------------------------
void LinuxAPI::updateDiskStats() {
   if (!diskDeviceFound) {
      ifstream mounts("/proc/mounts");
      if (mounts.is_open()) {
            string line;
            while (getline(mounts, line)) {
               istringstream iss(line);
               string dev, mountPoint, fstype, options, dump, pass;
               iss >> dev >> mountPoint >> fstype >> options >> dump >> pass;
               if (mountPoint == "/" && dev.find("/dev/") == 0) {
                  std::string base = dev.substr(5);
                  while (!base.empty() && isdigit(base.back()))
                        base.pop_back();
                  diskDevice = base;
                  diskDeviceFound = true;
                  break;
               }
            }
      }
      if (!diskDeviceFound) {
            diskDevice = "sda";
            diskDeviceFound = true;
      }
   }

   ifstream diskstat("/proc/diskstats");
   if (!diskstat.is_open()) {
      diskReadMBps = 0.0;
      diskWriteMBps = 0.0;
      return;
   }
   std::string line;
   unsigned long long readSectors = 0, writeSectors = 0;
   while (std::getline(diskstat, line)) {
      std::istringstream iss(line);
      int major, minor;
      std::string name;
      unsigned long long reads, readsMerged, sectorsRead, timeRead;
      unsigned long long writes, writesMerged, sectorsWrite, timeWrite;
      iss >> major >> minor >> name >> reads >> readsMerged >> sectorsRead >> timeRead
            >> writes >> writesMerged >> sectorsWrite >> timeWrite;
      if (name == diskDevice) {
            readSectors = sectorsRead;
            writeSectors = sectorsWrite;
            break;
      }
   }

   if (!firstDisk) {
      unsigned long long diffRead = readSectors - prevReadSectors;
      unsigned long long diffWrite = writeSectors - prevWriteSectors;
      double readMB = diffRead * 512.0 / (1024.0 * 1024.0);
      double writeMB = diffWrite * 512.0 / (1024.0 * 1024.0);
      diskReadMBps = readMB;
      diskWriteMBps = writeMB;
   }
   prevReadSectors = readSectors;
   prevWriteSectors = writeSectors;
   firstDisk = false;
   return;
}

// ----------------------------------------- Задержка в мкс ----------------------------------------
void LinuxAPI::usDelay(int delay) {
   usleep(delay);
   return;
}