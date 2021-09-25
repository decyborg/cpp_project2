#include "system.h"

#include <unistd.h>

#include <algorithm>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Constructor
System::System() {
  // Save kernel and OS info on private variables to avoid
  // parsing each time it is queried
  kernel_ = LinuxParser::Kernel();
  operating_system_ = LinuxParser::OperatingSystem();
}

// Done: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Done: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  // Get list of processes PIDs
  vector<int> pids = LinuxParser::Pids();
  // Use a hash map to check if the process is already in the list
  // and only add it if not
  // This implementations leaves dead processes
  for (int pid : pids) {
    // Add this Process if it is not in our vector
    if (pids_in_sys_.find(pid) == pids_in_sys_.end()) {
      pids_in_sys_.insert(pid);
      processes_.push_back(Process(pid));
    }
  }

  // Sort processes
  std::sort(processes_.begin(), processes_.end(), std::greater<Process>());
  return processes_;
}

// Done: Return the system's kernel identifier (string)
std::string System::Kernel() { return kernel_; }

// Done: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Done: Return the operating system name
std::string System::OperatingSystem() { return operating_system_; }

// Done: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Done: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Done: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
