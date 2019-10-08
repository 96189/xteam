// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//

#include <muduo/net/Buffer.h>

#include <muduo/net/SocketsOps.h>

#include <errno.h>
#include <sys/uio.h>

using namespace muduo;
using namespace muduo::net;

const char Buffer::kCRLF[] = "\r\n";

const size_t Buffer::kCheapPrepend;
const size_t Buffer::kInitialSize;

ssize_t Buffer::readFd(int fd, int* savedErrno)
{
  // saved an ioctl()/FIONREAD call to tell how much to read
  char extrabuf[65536];
  struct iovec vec[2];
  
  const size_t writable = writableBytes();  // std::vector 堆 可用空间
  vec[0].iov_base = begin()+writerIndex_;   // std::vector 堆
  vec[0].iov_len = writable;

  vec[1].iov_base = extrabuf;               // char 栈
  vec[1].iov_len = sizeof extrabuf;
  // when there is enough space in this buffer, don't read into extrabuf.
  // when extrabuf is used, we read 128k-1 bytes at most.
  const int iovcnt = (writable < sizeof extrabuf) ? 2 : 1;
  const ssize_t n = sockets::readv(fd, vec, iovcnt);  // 堆空间小于栈缓冲区空间 则数据分别读到堆和栈空间 否则数据只读到堆空间
  if (n < 0)  // 读取失败
  {
    *savedErrno = errno;
  }
  else if (implicit_cast<size_t>(n) <= writable)  // 堆空间未用完 已读完所有数据
  {
    writerIndex_ += n;
  }
  else  // 堆空间用完 数据没读完
  {
    writerIndex_ = buffer_.size();
    // 将栈缓冲区中的数据 转移到std::vector堆 堆需要扩展空间
    append(extrabuf, n - writable);
  }
  // if (n == writable + sizeof extrabuf)
  // {
  //   goto line_30;
  // }
  return n;
}

