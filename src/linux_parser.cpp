#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Parses a file looking for a key value pair and returns the value
std::optional<int> parse_kv(string dir, string key) {
  string value(""), tmp_key("");
  string line("");

  // Open File stream
  std::ifstream stream(dir);
  if (stream.is_open()) {
    // Check line by line for key
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> tmp_key >> value;
      if (tmp_key == key) {
        // Convert to int
        try {
          return std::stoi(value);
        } catch (std::invalid_argument const& ia) {
          std::cerr << "Invalid arg: " << ia.what() << "\n";
        }
      }
    }
  }

  return {};
}

// Parses a file word by word and returns the i-th word
std::optional<string> parse_words(string dir, int i_word) {
  string word;
  int count = 0;
  std::ifstream file(dir);

  // Iterate over file word by word
  while (std::getline(file, word, ' ')) {
    count++;
    if (count == i_word) return word;
  }

  return {};
}

float LinuxParser::TotalMemory() {
  auto value = parse_kv(kProcDirectory + kMeminfoFilename, "MemTotal:");
  if (value) return static_cast<float>(*value);
  return 0;
}

float LinuxParser::FreeMemory() {
  auto value = parse_kv(kProcDirectory + kMeminfoFilename, "MemFree:");
  if (value) return static_cast<float>(*value);
  return 0;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float total_memory = TotalMemory();
  float free_memory = FreeMemory();

  return (total_memory - free_memory) / total_memory;
}

// Done: Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime_str;
  long int uptime = -1;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    string tmp;
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime_str >> tmp;
  }
  try {
    uptime = stoi(uptime_str);
  } catch (std::invalid_argument const& ia) {
    std::cerr << "Invalid arg: " << ia.what() << "\n";
  }

  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// Done: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  long utime{0}, stime{0};
  long cutime{0}, cstime{0};

  string dir{kProcDirectory + to_string(pid) + kStatFilename};

  // Get utime
  auto val = parse_words(dir, 14);
  if (val) utime = stoi(*val);
  // Get stime
  val = parse_words(dir, 15);
  if (val) stime = stoi(*val);
  // Get cutime
  val = parse_words(dir, 16);
  if (val) cutime = stoi(*val);
  // Get cstime
  val = parse_words(dir, 17);
  if (val) cstime = stoi(*val);

  return utime + stime + cutime + cstime;
}

long LinuxParser::ProcActiveTime(int pid) {
  // Get active Jiffies
  long total = ActiveJiffies(pid);
  total /= sysconf(_SC_CLK_TCK);
  return total;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Done: Read and return CPU utilization
LinuxParser::CpuStats LinuxParser::CpuUtilization() {
  string cpu, user, nice, system, idle, iowait;
  string irq, softirq, steal, guest, guest_nice;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;
  }

  float idle_f = stof(idle) + stof(iowait);
  float nonidle_f = stof(user) + stof(nice) + stof(system) + stof(irq) +
                    stof(softirq) + stof(steal);

  CpuStats cpu_stats{idle_f, nonidle_f};
  return cpu_stats;
}

// Done: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  auto value = parse_kv(kProcDirectory + kStatFilename, "processes");
  if (value) return *value;
  return 0;
}

// Done: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  auto value = parse_kv(kProcDirectory + kStatFilename, "procs_running");
  if (value) return *value;
  return 0;
}

// Done: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
  }

  return string();
}

// Done: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  auto mem = parse_kv(kProcDirectory + std::to_string(pid) + kStatusFilename,
                      "VmSize:");
  if (mem) return std::to_string((*mem / 1000));
  return string();
}

// Done: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  auto uid =
      parse_kv(kProcDirectory + std::to_string(pid) + kStatusFilename, "Uid:");
  if (uid) return std::to_string(*uid);
  return string();
}

// Done: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);

  // Search for user
  string line;
  string user, tmp_uid;
  string x;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> tmp_uid) {
        if (tmp_uid == uid) {
          return user;
        }
      }
    }
  }

  return string();
}

// Done: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  // Get (22) starttime from proc/pid/stat
  auto val = parse_words(kProcDirectory + to_string(pid) + kStatFilename, 22);
  if (val) {
    long uptime = stoi(*val);
    // Convert from clock ticks to seconds
    uptime /= sysconf(_SC_CLK_TCK);
    return uptime;
  }

  return 0;
}
