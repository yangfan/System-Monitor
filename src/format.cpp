#include "format.h"

#include <iomanip>
#include <sstream>
#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int hours, minutes;
  minutes = seconds / 60;
  hours = minutes / 60;
  std::stringstream ss;

  ss << std::setw(2) << std::setfill('0') << hours << ":";
  // ss << ":";
  ss << std::setw(2) << std::setfill('0') << minutes % 60 << ":";
  // ss << ":";
  ss << std::setw(2) << std::setfill('0') << seconds % 60;

  return ss.str();
}