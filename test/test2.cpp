#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../include/linux_parser.h"

int main() {
  std::vector<int> vec = {1, 2, 3, 4};
  vec.clear();
  std::cout << vec.size() << std::endl;
  int pid = 23154;
  std::ifstream ifs(LinuxParser::kProcDirectory + std::to_string(pid) +
                    LinuxParser::kCmdlineFilename);
  std::string line;
  if (ifs.is_open()) {
    ifs >> line;
  }
  std::string cmd =
      "gzserver-eode/home/fanyang/catkin_ws/src/ros_sim/worlds/"
      "turtlebot3.world-s/opt/ros/melodic/lib/libgazebo_ros_paths_plugin.so-s/"
      "opt/ros/melodic/lib/libgazebo_ros_api_plugin.so__name:=gazebo__log:=/"
      "home/fanyang/.ros/log/029e111a-a227-11eb-b704-0028f8366721/gazebo-2.log";
  std::cout << line;
}