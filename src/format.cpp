#include "format.h"

#include <iomanip>
#include <sstream>
#include <string>

using std::string;

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

string Format::Number(int number, int digit) {
  std::stringstream ss;

  ss << std::setw(digit) << std::setfill(' ') << number;
  return ss.str();
}

std::string Format::NormalizeString(std::string input, size_t len) {
  if (len > input.size()) {
    std::string norm = input;
    for (size_t i = 0; i < len - input.size(); i++) {
      norm.append(" ");
    }
    return norm;
  }
  return input;
}
