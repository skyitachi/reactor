//
// Created by skyitachi on 2018/10/19.
//

#include "Poller.h"
#include "Channel.h"
#include <assert.h>
#include <glog/logging.h>
#include <absl/time/clock.h>

Poller::~Poller() {

}

void Poller::updateChannel(Channel* channel) {
  assertInLoopThread();
  LOG(INFO) << "fd = " << channel->fd() << " events = " << channel->events();
  if (channel->index() == -1) {
    assert(channels_.find(channel->fd()) == channels_.end());
    struct pollfd pfd;
    pfd.fd = channel->fd();
    pfd.events = static_cast<short>(channel->events());
    pfd.revents = 0;
    pollfds_.push_back(pfd);
    int idx = static_cast<int>(pollfds_.size() - 1);
    channel->set_index(idx);
    channels_[pfd.fd] = channel;
  } else {
    int idx = channel->index();
    assert(pollfds_.size() < idx);
    struct pollfd pfd = pollfds_[idx];
    pfd.events = channel->events();
  }
}

absl::Time Poller::poll(int timeoutMs, ChannelList* activeChannels) {
  assertInLoopThread();
  LOG(INFO) << "polllfds size: " << pollfds_.size();
  int numEvents = ::poll(&*pollfds_.begin(), pollfds_.size(), timeoutMs);
  absl::Time now = absl::Now();
  if (numEvents > 0) {
    LOG(INFO) << "numEvents: " << numEvents << " happended";
    // TODO: 填充channels
    fillActiveChannels(numEvents, activeChannels);
  } else if (numEvents == 0) {
    LOG(INFO) << "no events happpend";
  } else {
    LOG(ERROR) << "Poller::poll()";
  }
  return now;
}

void Poller::fillActiveChannels(int numEvents, ChannelList* activeChannels) {
  for (auto it = pollfds_.begin(); it != pollfds_.end(); it++) {
    Channel* channel = channels_[it->fd];
    if (channel->events() & it->revents) {
      channel->set_revents(it->revents);
      activeChannels->push_back(channel);
    }
  }
}