//
// Created by skyitachi on 2018/10/16.
//

#include "EventLoop.h"

// thread_local variable
__thread EventLoop* t_loopInThisThread = 0;

EventLoop::EventLoop() : looping_(false), threadId_(std::this_thread::get_id()) {
  DLOG(INFO) << "EventLoop created " <<  this << " in thread " << threadId_;
  if (t_loopInThisThread) {
    LOG(FATAL) << "thread already have event loop";
  } else {
    t_loopInThisThread = this;
  }
}

EventLoop::~EventLoop() {
  assert(!looping_);
  t_loopInThisThread  = NULL;
}

void EventLoop::abortNotInLoopThread() {
  LOG(FATAL)
    << "no run in same thread, created threadId " << threadId_
    << " running threadId " << std::this_thread::get_id();
}

EventLoop* EventLoop::getEventLoopOfCurrentThread() {
  return t_loopInThisThread;
}

void EventLoop::loop() {
  assert(!looping_);
  assertInLoopThread();
  looping_ = true;
  ::poll(NULL, 0, 5 * 1000);

  DLOG(INFO) << "EventLoop " << this << " stop looping";
  looping_ = false;
}
