#include "./../headers/LinuxAPI.h"

string LinuxAPI::getSystemName() {
   return "Linux";
}

// ------------------------------------------- Поиск PID -------------------------------------------
void LinuxAPI::updatePid() {
   DIR* dir = opendir("/proc");
   if (!dir) {
      this->pid = -1;
      return;
   }

   struct dirent* entry;
   while ((entry = readdir(dir)) != nullptr) {
      if (entry->d_type != DT_DIR) continue;
      const char* name = entry->d_name;
      char* endptr;
      long pid = strtol(name, &endptr, 10);
      if (*endptr != '\0' || pid <= 0) continue;

      string commPath = "/proc/" + string(name) + "/comm";
      ifstream commFile(commPath);
      if (!commFile.is_open()) continue;

      string comm;
      getline(commFile, comm);
      if (!comm.empty() && comm.back() == '\n') comm.pop_back();

      if (comm == nameApp) {
         closedir(dir);
         this->pid = static_cast<int>(pid);
         return;
      }
   }

   closedir(dir);
   this->pid = -1;
   return;
}

void LinuxAPI::updateData() {
   updatePid();
   updateSystemCpu();
   updateAppCpu();
   updateSystemMemory();
   updateAppMemory();
   updateTasksCount();
   updateThreadsCount();
   updateDiskStats();
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

// -------------------------------------- Чтение CPU процесса --------------------------------------
void LinuxAPI::updateAppCpu() {
   if (pid <= 0) {
      AppCpuUsagePercent = 0.0;
      return;
   }
   string statPath = "/proc/" + to_string(pid) + "/stat";
   ifstream statFile(statPath);
   if (!statFile.is_open()) {
      AppCpuUsagePercent = 0.0;
      return;
   }
   string line;
   getline(statFile, line);
   istringstream iss(line);
   vector<string> tokens;
   string token;
   while (iss >> token) tokens.push_back(token);
   if (tokens.size() < 24) {
      AppCpuUsagePercent = 0.0;
      return;
   }
   unsigned long long utime = stoull(tokens[13]);
   unsigned long long stime = stoull(tokens[14]);
   unsigned long long cutime = stoull(tokens[15]);
   unsigned long long cstime = stoull(tokens[16]);
   unsigned long long starttime = stoull(tokens[21]);

   ProcCpuStats cur{utime, stime, cutime, cstime, starttime};

   ifstream statSys("/proc/stat");
   if (!statSys.is_open()) {
      AppCpuUsagePercent = 0.0;
      return;
   }
   string lineSys;
   unsigned long long sysTotal = 0;
   while (getline(statSys, lineSys)) {
      if (lineSys.compare(0, 4, "cpu ") == 0) {
            istringstream issSys(lineSys.substr(4));
            unsigned long long val;
            while (issSys >> val) sysTotal += val;
            break;
      }
   }

   if (!firstProcCpu) {
      unsigned long long procDiff = (cur.utime + cur.stime) - (prevProcCpu.utime + prevProcCpu.stime);
      unsigned long long sysDiff = sysTotal - prevSysTotal;
      if (sysDiff > 0) {
            double usage = 100.0 * procDiff / sysDiff;
            AppCpuUsagePercent = usage;
      }
   }
   prevProcCpu = cur;
   prevSysTotal = sysTotal;
   firstProcCpu = false;
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

// ---------------------------------------- Память процесса ----------------------------------------
void LinuxAPI::updateAppMemory() {
   if (pid <= 0) {
      AppMemoryUsageMB = 0;
      return;
   }
   string statmPath = "/proc/" + to_string(pid) + "/statm";
   ifstream statm(statmPath);
   if (!statm.is_open()) {
      AppMemoryUsageMB = 0;
      return;
   }
   unsigned long long size, resident, share, text, lib, data, dt;
   statm >> size >> resident >> share >> text >> lib >> data >> dt;
   long pageSize = sysconf(_SC_PAGESIZE);
   if (pageSize > 0) {
      unsigned long long rssBytes = resident * pageSize;
      AppMemoryUsageMB = rssBytes / (1024 * 1024);
   } else {
      AppMemoryUsageMB = 0;
   }
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

// ---------------------------------- Количество потоков процесса ----------------------------------
void LinuxAPI::updateThreadsCount() {
   if (pid <= 0) {
      AppThreadsCount = 0;
      return;
   }
   string statusPath = "/proc/" + to_string(pid) + "/status";
   ifstream status(statusPath);
   if (!status.is_open()) {
      AppThreadsCount = 0;
      return;
   }
   string line;
   unsigned int threads = 0;
   while (getline(status, line)) {
      if (line.compare(0, 8, "Threads:") == 0) {
            istringstream iss(line);
            string key;
            iss >> key >> threads;
            break;
      }
   }
   AppThreadsCount = threads;
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