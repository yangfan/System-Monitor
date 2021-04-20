#include "processor.h"

#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "linux_parser.h"

using namespace std::this_thread;      // sleep_for, sleep_until
using namespace std::chrono_literals;  // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

Processor::Processor() {
  cputime_start_ = cputime();
  start_ = std::chrono::system_clock::now();
  sleep_for(1s);
  cputime_next_ = cputime();
}

std::vector<float> Processor::cputime() {
  std::vector<float> cputime;
  std::ifstream fst(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  std::string line, key, value;

  if (fst.is_open()) {
    while (std::getline(fst, line)) {
      std::stringstream ss(line);
      ss >> key;

      if (key == "cpu") {
        while (ss >> value) {
          cputime.emplace_back(std::stof(value));
        }
        break;
      }
    }
  }
  float idle = cputime[3] + cputime[4];
  float nonidle = cputime[0] + cputime[1] + cputime[2] + cputime[5] +
                  cputime[6] + cputime[7];
  float total = idle + nonidle;
  std::vector<float> cpudata{total, idle};

  return cpudata;
}

// Return the aggregate CPU utilization
float Processor::Utilization() {
  std::vector<float> cputime_now = cputime();
  std::chrono::time_point<std::chrono::system_clock> now =
      std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = now - start_;

  if (elapsed_seconds > time_interval_sec) {
    cputime_start_ = cputime_next_;
    cputime_next_ = cputime_now;
    start_ = now;
  }

  float total_now = cputime_now[0];
  float idle_now = cputime_now[1];
  float totald = total_now - cputime_start_[0];
  float idled = idle_now - cputime_start_[1];

  if (totald) {
    float cpu_percentage = (totald - idled) / totald;
    cpu_percentage_ = cpu_percentage;
  }

  return cpu_percentage_;
}
