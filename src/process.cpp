#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
  std::vector<float> cputime = LinuxParser::CpuUtilization(pid_);
  float utime = cputime[0], stime = cputime[1], cutime = cputime[2],
        cstime = cputime[3], starttime = cputime[4];
  float uptime = LinuxParser::UpTime();
  float total_time = utime + stime + cutime + cstime;
  float seconds = uptime - starttime / sysconf(_SC_CLK_TCK);

  return (total_time / sysconf(_SC_CLK_TCK)) / seconds;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) {
  //   return std::stoi(Ram()) > std::stoi(a.Ram());
  return CpuUtilization() > a.CpuUtilization();
}
// bool Process::operator<(Process const& a [[maybe_unused]]) { return true; }