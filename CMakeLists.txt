cmake_minimum_required(VERSION 3.12)
project(reactor)

set(CMAKE_CXX_STANDARD 14)

add_executable(reactor main.cpp EventLoop.cpp Channel.cpp Poller.cpp Poller.h)

target_link_libraries(reactor pthread glog absl_time)