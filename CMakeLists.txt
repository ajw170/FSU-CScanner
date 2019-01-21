cmake_minimum_required(VERSION 3.12)
project(Scanner)

set(CMAKE_CXX_STANDARD 14)
set(FLAGS  "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS  "${FLAGS}")

add_executable(Scanner scanner.cpp)