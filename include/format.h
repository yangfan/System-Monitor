#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);

std::string Number(int number, int digit);

std::string NormalizeString(std::string input, size_t len);

};  // namespace Format

#endif