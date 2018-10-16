//
// Created by skyitachi on 2018/10/16.
//

#ifndef REACTOR_NOCOPYABLE_H
#define REACTOR_NOCOPYABLE_H

class Nocopyable {
public:
  Nocopyable() = default;
  ~Nocopyable() = default;
  Nocopyable(const Nocopyable &) = delete;
  Nocopyable& operator=(const Nocopyable &) = delete;
};
#endif //REACTOR_NOCOPYABLE_H
