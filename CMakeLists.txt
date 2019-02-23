cmake_minimum_required(VERSION 3.13)
project(langust)

set(CMAKE_CXX_STANDARD 11)

include_directories(3rdparty/include)
include_directories(lgust/code/include)

add_subdirectory(repl)
add_subdirectory(lgust)
