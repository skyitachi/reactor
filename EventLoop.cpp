//
// Created by skyitachi on 2018/10/16.
//

#include "EventLoop.h"
#include "Channel.h"
#include "Poller.h"
#include <absl/time/clock.h>

// thread_local variable
__thread EventLoop* t_loopInThisThread = 0;

EventLoop::EventLoop() : looping_(false), threadId_(std::this_thread::get_id()) {
  LOG(INFO) << "EventLoop created " <<  this << " in thread " << threadId_;
  if (t_loopInThisThread) {
    LOG(FATAL) << "thread already have event loop";
  } else {
    t_loopInThisThread = this;
    poller_ = std::unique_ptr<Poller>(new Poller(this));
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
  quit_ = false;
  int cnt = 0;
  while (!quit_ && cnt++ < 3) {
    activeChannels_.clear();
    absl::Time now = absl::Now();
    absl::Time t = poller_->poll(5000, &activeChannels_);
    LOG(INFO) << "one poll consumes: " << t - now;
    for(auto it = activeChannels_.begin(); it != activeChannels_.end(); it++) {
      (*it)->handleEvent();
    }
  }
  DLOG(INFO) << "EventLoop " << this << " stop looping";
  looping_ = false;
}

void EventLoop::quit() {
  assertInLoopThread();
  quit_ = true;
}
// channel 只能在同一个 io线程中
void EventLoop::updateChannel(Channel* channel) {
  assert(channel->ownerLoop() == this);
  assertInLoopThread();
  poller_->updateChannel(channel);
}
