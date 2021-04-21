#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using std::stof;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;
namespace fs = std::filesystem;

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

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  std::vector<int> pids;
  for (auto& p : fs::directory_iterator(LinuxParser::kProcDirectory)) {
    std::string fn{p.path().filename()};
    if (std::all_of(fn.begin(), fn.end(), isdigit)) {
      int pid = stoi(fn);
      pids.push_back(pid);
    }
  }
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key, value, unit;
  std::unordered_map<string, float> mem_map;
  std::ifstream fstream(kProcDirectory + kMeminfoFilename);

  if (fstream.is_open()) {
    while (std::getline(fstream, line)) {
      std::stringstream strstream(line);
      strstream >> key >> value;

      if (key == "MemTotal:" || key == "MemFree:" || key == "Buffers:" ||
          key == "Cached:") {
        mem_map[key] = std::stof(value);
      }
    }

    if (mem_map.size() == 4) {
      return (mem_map["MemTotal:"] - mem_map["MemFree:"] - mem_map["Cached:"] -
              mem_map["Buffers:"]) /
             mem_map["MemTotal:"];
    }
  }

  return 0.0;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  std::ifstream ifs(kProcDirectory + kUptimeFilename);
  std::string uptime_str;

  if (ifs.is_open()) {
    ifs >> uptime_str;
    return std::stol(uptime_str);
  }

  return 0;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream ifs(kProcDirectory + kStatFilename);
  string line, key, value;
  int num_proc = 0;
  if (ifs.is_open()) {
    while (std::getline(ifs, line)) {
      std::stringstream ss(line);
      ss >> key >> value;
      if (key == "processes") {
        num_proc = stoi(value);
        break;
      }
    }
  }
  return num_proc;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream ifs(kProcDirectory + kStatFilename);
  string line, key, value;
  int run_proc = 0;
  std::stringstream oss;
  if (ifs.is_open()) {
    while (std::getline(ifs, line)) {
      std::stringstream ss(line);
      ss >> key >> value;
      if (key == "procs_running") {
        oss << std::setw(5) << std::setfill('0') << value;
        oss >> run_proc;
        break;
      }
    }
  }
  ifs.close();
  return run_proc;
}
// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::ifstream ifs(kProcDirectory + to_string(pid) + kCmdlineFilename);
  string line = "";
  if (ifs.is_open()) {
    std::getline(ifs, line);
  }
  return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  std::ifstream ifs(kProcDirectory + to_string(pid) + kStatusFilename);
  string line, key, mem_str{"0"};
  if (ifs.is_open()) {
    while (std::getline(ifs, line)) {
      std::stringstream ss(line);
      ss >> key;
      if (key == "VmSize:") {
        ss >> mem_str;
        break;
      }
    }
  }
  ifs.close();
  int mem_int = std::stoi(mem_str) / 1024;
  return to_string(mem_int);
  // return mem_str;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::ifstream ifs(kProcDirectory + to_string(pid) + kStatusFilename);
  string line, key, uid_str;
  if (ifs.is_open()) {
    while (std::getline(ifs, line)) {
      std::stringstream ss(line);
      ss >> key;
      if (key == "Uid:") {
        ss >> uid_str;
        break;
      }
    }
  }
  ifs.close();
  return uid_str;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid_str = Uid(pid);
  string line, name, letter, value, user_name;
  std::ifstream ifs(kPasswordPath);
  if (ifs.is_open()) {
    while (std::getline(ifs, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::stringstream ss(line);
      ss >> name >> letter >> value;
      if (value == uid_str) {
        user_name = name;
        break;
      }
    }
  }
  ifs.close();
  return user_name;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  std::ifstream ifs(kProcDirectory + to_string(pid) + kStatFilename);
  string value{"0"};
  if (ifs.is_open()) {
    for (int i = 1; i < 23; i++) {
      ifs >> value;
    }
  }
  long proc_tck = std::stol(value);
  long proc_sec = proc_tck / sysconf(_SC_CLK_TCK);
  return proc_sec;
}

// Read and return CPU utilization
vector<float> LinuxParser::CpuUtilization(int pid) {
  std::ifstream ifs(kProcDirectory + to_string(pid) + kStatFilename);
  std::vector<float> vec_time;

  string value{"0"};
  if (ifs.is_open()) {
    for (int i = 1; i < 23; i++) {
      ifs >> value;
      if (i == 14 || i == 15 || i == 16 || i == 17 || i == 22) {
        vec_time.push_back(stof(value));
      }
    }
  }

  return vec_time;
}
