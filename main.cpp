#include <iostream>
#include <glog/logging.h>
#include "EventLoop.h"
#include <stdio.h>

EventLoop* g_loop;

void threadFunc() {
  LOG(INFO) << "in the thread " << std::this_thread::get_id();
  EventLoop loop;
  loop.loop();
}

void threadTest() {
  LOG(INFO) << "in the thread " << std::this_thread::get_id();
  g_loop->loop();
}

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  FLAGS_alsologtostderr = true;
  printf("main(): pid = %d, tid = %d\n", getpid(), std::this_thread::get_id());
//  std::thread t1(threadFunc);
//  EventLoop loop;
//  loop.loop();
//  t1.join();
  // crash
  EventLoop loop;
  g_loop = &loop;
  std::thread t1(threadTest);
  t1.join();
  return 0;
}