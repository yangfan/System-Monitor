#include <chrono>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <vector>

#include "../include/linux_parser.h"
#include "../include/system.h"
int main() {
  enum CPUStates {
    kUser_ = 0,
    kNice_,
    kSystem_,
    kIdle_,
    kIOwait_,
    kIRQ_,
    kSoftIRQ_,
    kSteal_,
    kGuest_,
    kGuestNice_
  };
  std::vector<int> vec;
  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  std::cout << vec[kUser_] << " " << vec[kGuestNice_] << " " << vec[kSystem_]
            << std::endl;
  std::cout << kGuestNice_ << std::endl;
  std::cout << vec[9] << std::endl;
  // vec[kUser_] = 1;
  // vec[kNice_] = 2;
  // vec[kSystem_] = 3;
  for (const auto ele : vec) {
    std::cout << ele << std::endl;
  }
}