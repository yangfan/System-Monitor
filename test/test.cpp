#include <chrono>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <vector>

#include "../include/linux_parser.h"
#include "../include/system.h"
int main() {
  std::string line{"abd:      123"};
  std::stringstream ss(line);
  std::string word1, word2, word3;
  ss >> word1 >> word2 >> word3;
  std::cout << word1 << " \n" << word2 << " \n" << word3 << std::endl;
  return 0;
}