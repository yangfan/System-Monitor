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

Process::Process(int pid) : pid_(pid) {
  user_ = LinuxParser::User(pid_);
  command_ = LinuxParser::Command(pid_);
  ram_ = LinuxParser::Ram(pid_);
  uptime_ = LinuxParser::UpTime(pid_);
  cpuUsage_ = compute_cpu();
}

float Process::compute_cpu() {
  std::vector<float> cputime = LinuxParser::CpuUtilization(pid_);
  if (cputime.size() == 5) {
    float utime = cputime[0], stime = cputime[1], cutime = cputime[2],
          cstime = cputime[3], starttime = cputime[4];
    float uptime = LinuxParser::UpTime();
    float total_time = utime + stime + cutime + cstime;
    float seconds = uptime - starttime / sysconf(_SC_CLK_TCK);

    return (total_time / sysconf(_SC_CLK_TCK)) / seconds;
  }
  return 0;
}
// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpuUsage_; }

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process& a) {
  //   return std::stoi(Ram()) > std::stoi(a.Ram());
  return CpuUtilization() > a.CpuUtilization();
}
// bool Process::operator<(Process const& a [[maybe_unused]]) { return true; }