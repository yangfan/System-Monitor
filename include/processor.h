#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <chrono>
#include <vector>

class Processor {
 public:
  Processor();
  std::vector<float> cputime();
  float Utilization();

 private:
  std::chrono::seconds time_interval_sec = std::chrono::seconds(5);
  std::vector<float> cputime_start_;
  std::vector<float> cputime_next_;
  std::chrono::time_point<std::chrono::system_clock> start_;
  float cpu_percentage_;
};

#endif