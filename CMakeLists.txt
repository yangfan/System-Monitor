cmake_minimum_required(VERSION 3.0)
project(monitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(${CURSES_INCLUDE_DIRS})
include_directories(include/)

set(RESULTS_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/results)
file(MAKE_DIRECTORY ${RESULTS_OUTPUT_DIRECTORY})
file(GLOB SOURCES "src/*.cpp")


find_package(Curses REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RESULTS_OUTPUT_DIRECTORY})

add_executable(monitor ${SOURCES})

target_link_libraries(monitor ${CURSES_LIBRARIES})