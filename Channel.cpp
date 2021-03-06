//
// Created by skyitachi on 2018/10/18.
//

#include <poll.h>
#include "Channel.h"
#include "EventLoop.h"

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop *loop, int fd)
  : loop_(loop),
    fd_(fd),
    events_(kNoneEvent),
    revents_(kNoneEvent),
    index_(-1)
{

}

void Channel::update() {
  loop_->updateChannel(this);
}

void Channel::handleEvent() {
  if ((revents_ & kReadEvent) && readCallback_ != NULL) {
    LOG(INFO) << "handle read event";
    readCallback_();
  }
  LOG(INFO) << "in the handleEvent for write";
  if ((revents_ & kWriteEvent) && writeCallback_ != NULL) {
    LOG(INFO) << "handle write event";
    writeCallback_();
  }
}
