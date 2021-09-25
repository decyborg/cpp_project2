#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

// Constructor
Process::Process(int pid) : pid_(pid) {}

// Done: Return this process's ID
int Process::Pid() { return pid_; }

// Done: Return this process's CPU utilization
float Process::CpuUtilization() const {
  float cpu_usage{0};

  // Get total time spent on the process in seconds
  long total = LinuxParser::ProcActiveTime(pid_);

  // Get Process starttime in seconds
  long starttime = LinuxParser::UpTime(pid_);

  // Get system Up Time
  long uptime = LinuxParser::UpTime();

  long seconds = uptime - starttime;
  if (seconds != 0)
    cpu_usage = (static_cast<float>(total) / static_cast<float>(seconds));
  return cpu_usage;
}

// Done: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Done: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Done: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// Done: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// Done: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return this->CpuUtilization() < a.CpuUtilization();
}

// Done: Overload the "greater than" comparison operator for Process objects
bool Process::operator>(Process const& a) const {
  return this->CpuUtilization() > a.CpuUtilization();
}
