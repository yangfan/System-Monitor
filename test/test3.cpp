#include <iostream>
#include <string>
#include <vector>

std::string NormalizeString(std::string input, size_t len) {
  size_t diff = len - input.size();
  if (diff > 0) {
    for (size_t i = 0; i < diff; i++) {
      input.append("_");
    }
  }
  return input;
}
int main() {
  std::string line{"abc"};
  std::string ans = NormalizeString(line.substr(0, 10), 10);
  std::cout << ans.c_str() << std::endl;
  std::string ana = NormalizeString(line.substr(0, 10).c_str(), 10);
  std::cout << ana << std::endl;

  std::string test = line;
  std::cout << test.size() << std::endl;

  std::string test1 = line.c_str();
  std::cout << test1.size() << std::endl;

  return 0;
}