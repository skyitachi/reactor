//
// Created by skyitachi on 2018/10/16.
//

#ifndef REACTOR_EVENTLOOP_H
#define REACTOR_EVENTLOOP_H
#include "Nocopyable.h"
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <thread>

class EventLoop: Nocopyable {
public:
  EventLoop();
  ~EventLoop();
  void loop();
  void assertInLoopThread() {
    if (!isInLoopThread()) {
      abortNotInLoopThread();
    }
  }
  bool isInLoopThread() const {
    return threadId_ == std::this_thread::get_id();
  }

private:
  const std::thread::id threadId_;
  void abortNotInLoopThread();
  bool looping_;
};


#endif //REACTOR_EVENTLOOP_H
