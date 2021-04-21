#include <iostream>

#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;
  NCursesDisplay::Display(system);
  // while (true) {
  //   std::string str;
  //   int i = 1;
  //   for (auto p : system.Processes()) {
  //     if (i == 11) {
  //       break;
  //     }
  //     std::cout << p.Command() << std::endl;
  //   }
  // }
}