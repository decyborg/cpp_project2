#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <unordered_set>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  Processor& Cpu();                   // Done: See src/system.cpp
  std::vector<Process>& Processes();  // Done: See src/system.cpp
  float MemoryUtilization();          // Done: See src/system.cpp
  long UpTime();                      // Done: See src/system.cpp
  int TotalProcesses();               // Done: See src/system.cpp
  int RunningProcesses();             // Done: See src/system.cpp
  std::string Kernel();               // Done: See src/system.cpp
  std::string OperatingSystem();      // Done: See src/system.cpp

  // Done: Define any necessary private members
 private:
  Processor cpu_ = {};
  std::unordered_set<int> pids_in_sys_ = {};
  std::vector<Process> processes_ = {};
  std::string kernel_;
  std::string operating_system_;
};

#endif
