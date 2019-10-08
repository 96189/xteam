// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_TIMERQUEUE_H
#define MUDUO_NET_TIMERQUEUE_H

#include <set>
#include <vector>

#include <boost/noncopyable.hpp>

#include <muduo/base/Mutex.h>
#include <muduo/base/Timestamp.h>
#include <muduo/net/Callbacks.h>
#include <muduo/net/Channel.h>

namespace muduo
{
namespace net
{

class EventLoop;
class Timer;
class TimerId;

///
/// A best efforts timer queue.
/// No guarantee that the callback will be on time.
///
class TimerQueue : boost::noncopyable
{
 public:
  explicit TimerQueue(EventLoop* loop);
  ~TimerQueue();

  ///
  /// Schedules the callback to be run at given time,
  /// repeats if @c interval > 0.0.
  ///
  /// Must be thread safe. Usually be called from other threads.
  // cb 定时器回调 when超时绝对时间戳 interval周期性间隔
  TimerId addTimer(const TimerCallback& cb, Timestamp when, double interval);
#ifdef __GXX_EXPERIMENTAL_CXX0X__
  TimerId addTimer(TimerCallback&& cb,
                   Timestamp when,
                   double interval);
#endif

  // 取消定时器
  void cancel(TimerId timerId);

 private:

  // FIXME: use unique_ptr<Timer> instead of raw pointers.
  // This requires heterogeneous comparison lookup (N3465) from C++14
  // so that we can find an T* in a set<unique_ptr<T>>.
  typedef std::pair<Timestamp, Timer*> Entry;
  typedef std::set<Entry> TimerList;              // 插入时元素自动排序
  typedef std::pair<Timer*, int64_t> ActiveTimer;
  typedef std::set<ActiveTimer> ActiveTimerSet;   // 插入时元素自动排序

  void addTimerInLoop(Timer* timer);
  void cancelInLoop(TimerId timerId);
  // called when timerfd alarms
  void handleRead();
  // move out all expired timers
  std::vector<Entry> getExpired(Timestamp now);
  void reset(const std::vector<Entry>& expired, Timestamp now);

  bool insert(Timer* timer);

  EventLoop* loop_;
  const int timerfd_;             // 定时器事件描述符
  Channel timerfdChannel_;        // 定时器事件通道
  // Timer list sorted by expiration
  TimerList timers_;              // 定时器列表

  // for cancel()
  ActiveTimerSet activeTimers_;   // 活跃定时器列表
  bool callingExpiredTimers_; /* atomic */
  ActiveTimerSet cancelingTimers_;
};

}
}
#endif  // MUDUO_NET_TIMERQUEUE_H
