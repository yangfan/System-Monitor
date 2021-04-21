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

System::System() { Read(); }

void System::Read() {
  // processes_.clear();
  std::vector<Process> processes;
  std::vector<int> pids = LinuxParser::Pids();
  for (int pid : pids) {
    Process process(pid);
    if (std::stoi(process.Ram()) > 0) {
      processes.push_back(process);
    }
  }
  processes_ = processes;
  // std::sort(processes_.begin(), processes_.end());

  cpu_ = Processor();
  pids_ = pids;
}

void System::Update() {
  std::vector<int> pids = LinuxParser::Pids();
  // if (pids.size() != pids_.size()) {
  //   Read();
  // } else {
  // std::sort(processes_.begin(), processes_.end());
  // }
}

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
// vector<Process>& System::Processes() {
//   Update();
//   // std::sort(processes_.begin(), processes_.end());
//   return processes_;
// }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<Process> foundProcesses{};
  // read process IDs from file system and generate Vector
  vector<int> processIds = LinuxParser::Pids();
  for (int p : processIds) {
    Process pro{p};
    foundProcesses.push_back(pro);
  }

  // sort the processes according to their CPU usage
  sort(foundProcesses.begin(), foundProcesses.end(),
       [](Process& pa, Process& pb) {
         return (pb.CpuUtilization() < pa.CpuUtilization());
       });
  // update list of processes
  processes_ = foundProcesses;

  return processes_;
}
// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }