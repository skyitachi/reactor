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
#include <glog/logging.h>
#include <poll.h>
#include <vector>
#include <memory>

class Channel;
class Poller;
class EventLoop: Nocopyable {
public:
  EventLoop();
  ~EventLoop();
  void loop();
  EventLoop* getEventLoopOfCurrentThread();
  void assertInLoopThread() {
    if (!isInLoopThread()) {
      abortNotInLoopThread();
    }
  }
  bool isInLoopThread() const {
    return threadId_ == std::this_thread::get_id();
  }

  void updateChannel(Channel* channel);

private:
  typedef std::vector<Channel *> ChannelList;
  const std::thread::id threadId_;
  void abortNotInLoopThread();
  bool looping_;
  bool quit_;
  std::unique_ptr<Poller> poller_;
  ChannelList activeChannels_;
};


#endif //REACTOR_EVENTLOOP_H
