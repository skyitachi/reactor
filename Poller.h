//
// Created by skyitachi on 2018/10/19.
//

#ifndef REACTOR_POLLER_H
#define REACTOR_POLLER_H

#include <vector>
#include <map>
#include <absl/time/Time.h>

#include "Nocopyable.h"
#include "EventLoop.h"

struct pollfd;

class Channel;
class Poller: Nocopyable {
public:
  typedef std::vector<Channel*> ChannelList;
  Poller(EventLoop *loop): ownerLoop_(loop) {}
  ~Poller();

  // Poll
  absl::Time poll(int timeoutMs, ChannelList* activeChannels);

  void updateChannel(Channel* channel);

  void assertInLoopThread() {
    LOG(INFO) << "in the poller assert " << ownerLoop_;
    ownerLoop_->assertInLoopThread();
  }

private:
  void fillActiveChannels(int numEvents, ChannelList* activeChannels);
  typedef std::vector<struct pollfd> PollFdList;
  typedef std::map<int, Channel*> ChannelMap;

  EventLoop* ownerLoop_;
  PollFdList pollfds_;
  ChannelMap channels_;

};


#endif //REACTOR_POLLER_H
