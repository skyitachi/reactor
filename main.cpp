#include <iostream>
#include <glog/logging.h>
#include "EventLoop.h"
#include "Channel.h"
#include "Poller.h"
#include <stdio.h>
#include <fcntl.h>

EventLoop* g_loop;
Channel* g_channel;

void threadFunc() {
  LOG(INFO) << "in the thread " << std::this_thread::get_id();
  EventLoop loop;
  loop.loop();
}

void threadTest() {
  LOG(INFO) << "in the thread " << std::this_thread::get_id();
  g_loop->loop();
}

void readCallback() {
  LOG(INFO) << "in the readcallback";
  // g_loop->quit();
}

void writecb() {
  LOG(INFO) << "in the write callback";
  g_channel->disableAll();
  // g_loop->quit();
}

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  FLAGS_alsologtostderr = true;
  LOG(INFO) << "main(): pid = " << getpid() << " tid = " << std::this_thread::get_id();
//  std::thread t1(threadFunc);
//  EventLoop loop;
//  loop.loop();
//  t1.join();

  // crash
  // EventLoop loop;
  // g_loop = &loop;
  // std::thread t1(threadTest);
  // t1.join();
  int fd = open("./build.ninja", O_RDWR);
  EventLoop loop;
  g_loop = &loop;
  Channel channel(&loop, fd); 
  g_channel = &channel;
  channel.setReadCallback(readCallback);
  channel.setWriteCallback(writecb);
  channel.enableReading();
  channel.enableWriting();
  loop.loop();
  return 0;
}