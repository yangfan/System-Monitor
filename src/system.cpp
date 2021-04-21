#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <iostream>
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

System::System() : cpu_(Processor()) { Read(); }

// read processes
void System::Read() {
  std::vector<Process> processes;
  std::vector<int> pids = LinuxParser::Pids();
  for (int pid : pids) {
    Process process(pid);
    if (std::stoi(process.Ram()) > 0) {
      processes.push_back(process);
    }
  }
  std::sort(processes.begin(), processes.end());
  std::sort(pids.begin(), pids.end());
  processes_ = processes;
  pids_ = pids;
}

// update processes if changed
void System::Update() {
  std::vector<int> pids = LinuxParser::Pids();
  std::sort(pids.begin(), pids.end());
  if (pids != pids_) {
    Read();
  }
}

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
  Update();
  return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }