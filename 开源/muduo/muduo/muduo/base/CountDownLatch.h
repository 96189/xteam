// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_COUNTDOWNLATCH_H
#define MUDUO_BASE_COUNTDOWNLATCH_H

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>

#include <boost/noncopyable.hpp>

namespace muduo
{

class CountDownLatch : boost::noncopyable
{
 public:

  explicit CountDownLatch(int count); // 初始化计数器(主线程调用,用其他需要等待的其他线程总个数来初始化主线程)

  void wait();        // 计数器大于0 条件变量持续等待(主线程调用)

  void countDown();   // 计数器递减 计数器等于0 唤醒等待在条件变量上的线程(其他线程调用)

  int getCount() const;

 private:
  mutable MutexLock mutex_;
  Condition condition_ GUARDED_BY(mutex_);
  int count_ GUARDED_BY(mutex_);            // 计数器
};

}
#endif  // MUDUO_BASE_COUNTDOWNLATCH_H
