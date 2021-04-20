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

// DONE: An example of how to read data from the filesystem
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

//  Update this to use std::filesystem
// vector<int> LinuxParser::Pids() {
//   vector<int> pids;
//   DIR* directory = opendir(kProcDirectory.c_str());
//   struct dirent* file;
//   while ((file = readdir(directory)) != nullptr) {
//     // Is this a directory?
//     if (file->d_type == DT_DIR) {
//       // Is every character of the name a digit?
//       string filename(file->d_name);
//       if (std::all_of(filename.begin(), filename.end(), isdigit)) {
//         int pid = stoi(filename);
//         pids.push_back(pid);
//       }
//     }
//   }
//   closedir(directory);
//   return pids;
// }

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

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

std::unordered_map<string, long> LinuxParser::Stat() {
  std::unordered_map<string, long> procs_map;
  std::ifstream ifs(kProcDirectory + kStatFilename);
  string line, key, value;
  if (ifs.is_open()) {
    while (std::getline(ifs, line)) {
      std::stringstream ss(line);
      ss >> key >> value;
      if (key == "processes" || key == "procs_running") {
        procs_map[key] = std::stol(value);
      }
      if (procs_map.size() == 2) {
        break;
      }
    }
  }
  return procs_map;
}
// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::unordered_map<string, long> procs_map = Stat();
  return procs_map["processes"];
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::unordered_map<string, long> procs_map = Stat();
  return procs_map["procs_running"];
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
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

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
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

  // std::cout << uid_str;
  string name, letter, value, user_name;
  ifs.open(kPasswordPath);
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
  return user_name;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }
