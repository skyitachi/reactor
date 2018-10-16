//
// Created by skyitachi on 2018/10/16.
//

#include "EventLoop.h"

// thread_local variable
__thread EventLoop* t_loopInThisThread = 0;

EventLoop::EventLoop()
  : looping_(false),
    threadId_(std::this_thread::get_id()) {
    if (t_loopInThisThread) {

    } else {
      t_loopInThisThread = this;
    }
  }
