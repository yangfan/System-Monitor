#include "process.h"

#include <unistd.h>

#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
  user_ = LinuxParser::Uid(pid_);
  ram_ = LinuxParser::Ram(pid_);
}

// Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return 0; }

// Return the command that generated this process
string Process::Command() const { return string(); }

// Return this process's memory utilization
string Process::Ram() const { return ram_; }

// Return the user (name) that generated this process
string Process::User() const { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return std::stoi(ram_) > std::stoi(a.Ram());
}