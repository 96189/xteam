
##   I/O复用

[彻底理解I/O复用](https://juejin.cn/post/6882984260672847879)

+ 问题

    * 什么是I/O复用？为什么有I/O复用？I/O复用解决什么问题？

    * I/O复用的实现方式？主流I/O复用实现对比

    * 开源软件中对I/O复用的使用？

+   select(查询式)
    *   [内核源码剖析](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/linux/3.10.0-957.el7/fs/select.c)

        调用链
        
        ```
        select
            ->  core_sys_select
                        ->  do_select
        ```

+   epoll(触发式)
    *   ET与LT区别及编程注意事项

        epoll中epoll_event的events(事件类型)默认是LT模式,要使用ET模式需要主动为事件添加(|)EPOLLET标记.
        
        LT模式当事件就绪时发出通知后,若数据未处理完,则会反复通知.
        
        ET模式事件就绪时只通知一次,不管数据是否处理完成,本次触发的事件只通知一次,需要程序员循环处理数据直到完成.
    *   [内核源码剖析](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/linux/3.10.0-957.el7/fs/eventpoll.c)
        
        调用链
            
            epoll_wait
                ->  ep_poll 
                        ->  ep_send_events 
                                ->  ep_scan_ready_list 
                                            ->  ep_send_events_proc
        ```
        static int ep_send_events_proc(struct eventpoll *ep, struct list_head *head, void *priv) {
            for (eventcnt = 0,uevent = esed->events; !list_empty(head) && eventcnt < esed->maxevents; ) {
                if (revents) {
                    if (epi->event.events & EPOLLONESHOT)
                        epi->event.events &= EP_PRIVATE_BITS;
                    // LT模式
                    else if (!(epi->event.events & EPOLLET)) {
                        /*
                        * If this file has been added with Level
                        * Trigger mode, we need to insert back inside
                        * the ready list, so that the next call to
                        * epoll_wait() will check again the events
                        * availability. At this point, no one can insert
                        * into ep->rdllist besides us. The epoll_ctl()
                        * callers are locked out by
                        * ep_scan_ready_list() holding "mtx" and the
                        * poll callback will queue them in ep->ovflist.
                        */
                        // 重新放入就绪链表
                        list_add_tail(&epi->rdllink, &ep->rdllist);
                        ep_pm_stay_awake(epi);
                    }
                }
            }
        }
        ```

        struct eventpoll

+   对比

    select问题在于1、可监控的最大描述符个数有限制 2、每次监听事件需要在用户空间与内核空间来回拷贝事件结构 3、只通知事件就绪,具体哪些事件就绪需要通过轮询得知。

    epoll解决了上面的3个问题,得益于内核数据结构的设计应用RB-tree和双向链表来管理事件,通过mmap使得内核空间和用户空间共享一块内存空间,减少数据拷贝.

    并发量大,但同时活跃连接少的情况下,epoll更高效.并发小同时活跃连接较多的情况下,select更高效.(此情况下epoll的回调机制带来一部分损耗)

## Java NIO与Netty
+   javaNIO核心
    * Channels
    
            在NIO编程模型中,所有的I/O以Channel的形式,Channel是一种I/O通道类似与stream.应用程序从Channel中读请求取数据到Buffer,将Buffer中的响应数据写入Channel返回给客户端.

        *   java中的Channel
                
                DatagramChannel -> UDP
                ServerSocketChannel -> TCP -> listen
                SocketChannel -> TCP -> connection
                FileChannel -> local



    * Selectors

            每线程一个reactor事件循环,I/O多路复用(select,kqueue,epoll)高级别封装,可以将多个Channel注册到指定的selector中监听(epoll_wait).若已注册Channel事件就绪,则分发事件处理.

    * Buffers

            内存缓冲区封装
+   Netty核心组件

        注册回调事件驱动

    * channel

    * callback

    * event & channelHandler

    * future