
## 网络IO ##
### epoll ###
    tars的epoll默认采用ET模式

* struct epoll_event
```
struct epoll_event
{
    uint32_t events;      /* Epoll events */
    epoll_data_t data;    /* User data variable */
} __attribute__ ((__packed__));

typedef union epoll_data
{
    void *ptr;
    int fd;
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;
```
* 事件定义
    ```
    //定义事件类型
    enum
    {
        ET_LISTEN = 1,
        ET_CLOSE  = 2,
        ET_NOTIFY = 3,
        ET_NET    = 0,
    };
    ```
* 描述符与事件关联 

套接字监听事件
```
void TC_EpollServer::NetThread::createEpoll(uint32_t iIndex)
{
    if(!_createEpoll)
    {
        _createEpoll = true;

        // 创建本网络线程的内存池
        assert (!_bufferPool);
        _bufferPool = new TC_BufferPool(_poolMinBlockSize, _poolMaxBlockSize);
        _bufferPool->SetMaxBytes(_poolMaxBytes);

        //创建epoll
        _epoller.create(10240);

        // 管道(用于关闭服务) 高32位为事件类型
        _epoller.add(_shutdown.getfd(), H64(ET_CLOSE), EPOLLIN);
        // 管道(用于通知有数据需要发送就)
        _epoller.add(_notify.getfd(), H64(ET_NOTIFY), EPOLLIN);

        size_t maxAllConn   = 0;

        //监听socket
        for (const auto& kv : _listeners)
        {
            if(kv.second->getEndpoint().isTcp())
            {
                //获取最大连接数
                maxAllConn += kv.second->getMaxConns();
                // 高32位为事件类型 低32位为套接字描述符
                _epoller.add(kv.first, H64(ET_LISTEN) | kv.first, EPOLLIN);
            }
            else
            {
                maxAllConn++;
                _hasUdp = true;
            }
        }

        if(maxAllConn == 0)
        {
            //当网络线程中listeners没有监听socket时，使用adapter中设置的最大连接数
            maxAllConn = _listSize;
        }

        // 限制最大连接数(设置为系统最大上限值)
        if(maxAllConn >= (1 << 22))
        {
            error("createEpoll connection num: " + TC_Common::tostr(maxAllConn) + " >= " + TC_Common::tostr(1 << 22));
            maxAllConn = (1 << 22) - 1;
        }

        //初始化连接管理链表
        _list.init(maxAllConn, iIndex);
    }
}
```

连接关闭事件
```
void TC_EpollServer::NetThread::terminate()
{
    _bTerminate = true;

    //通知队列醒过来
    _sbuffer.notifyT();

    //通知epoll响应, 关闭连接 高32为为事件类型
    _epoller.mod(_shutdown.getfd(), H64(ET_CLOSE), EPOLLOUT);
}
```

连接通知事件
```
void TC_EpollServer::NetThread::close(uint32_t uid)
{
    tagSendData* send = new tagSendData();

    send->uid = uid;

    send->cmd = 'c';

    _sbuffer.push_back(send);

    //通知epoll响应, 关闭连接 高32位为事件类型
    _epoller.mod(_notify.getfd(), H64(ET_NOTIFY), EPOLLOUT);
}
void TC_EpollServer::NetThread::send(uint32_t uid, const string &s, const string &ip, uint16_t port)
{
    if(_bTerminate)
    {
        return;
    }

    tagSendData* send = new tagSendData();

    send->uid = uid;

    send->cmd = 's';

    send->buffer = s;

    send->ip = ip;

    send->port = port;

    _sbuffer.push_back(send);

    //通知epoll响应, 有数据要发送 高32位为事件类型
    _epoller.mod(_notify.getfd(), H64(ET_NOTIFY), EPOLLOUT);
}
```

### 网络事件分发 ###
void TC_EpollServer::NetThread::run()
{
    //循环监听网路连接请求
    while(!_bTerminate)
    {
        _list.checkTimeout(TNOW);

        int iEvNum = _epoller.wait(2000);

        for(int i = 0; i < iEvNum; ++i)
        {
            try
            {
                const epoll_event &ev = _epoller.get(i);
                // 取高32位获取事件类型
                uint32_t h = ev.data.u64 >> 32;
                switch(h)
                {
                // 监听连接上有新连接建立请求
                case ET_LISTEN:
                    {
                        //监听端口有请求
                        auto it = _listeners.find(ev.data.u32);
                        if( it != _listeners.end())
                        {
                            if(ev.events & EPOLLIN)
                            {
                                bool ret;
                                do
                                {
                                    // 获取新连接
                                    ret = accept(ev.data.u32, it->second->_ep.isIPv6() ? AF_INET6 : AF_INET);
                                } while(ret);
                            }
                        }
                    }
                    break;
                case ET_CLOSE:
                    //关闭请求
                    break;
                case ET_NOTIFY:
                    //发送通知
                    processPipe();
                    break;
                // 已建立的连接
                case ET_NET:
                    //网络请求
                    processNet(ev);
                    break;
                default:
                    assert(true);
                }
            }
            catch(exception &ex)
            {
                error("run exception:" + string(ex.what()));
            }
        }
    }
}

* 读写请求处理

```
void TC_EpollServer::NetThread::processNet(const epoll_event &ev)
{
    // 当前连接的描述符fd(低32位)
    uint32_t uid = ev.data.u32;
    Connection *cPtr = getConnectionPtr(uid);

    if (ev.events & EPOLLERR || ev.events & EPOLLHUP)
    {
        delConnection(cPtr,true,EM_SERVER_CLOSE);
        return;
    }

    if(ev.events & EPOLLIN)               //有数据需要读取
    {
        recv_queue::queue_type vRecvData;
        int ret = recvBuffer(cPtr, vRecvData);
        if(!vRecvData.empty())
        {
            cPtr->insertRecvQueue(vRecvData);
        }
    }

    if (ev.events & EPOLLOUT)              //有数据需要发送
    {
        int ret = sendBuffer(cPtr);
    }

    // uid描述符fd 刷新连接管理链表
    _list.refresh(uid, cPtr->getTimeout() + TNOW);
}
```

### 收包入口 ##
```
int TC_EpollServer::NetThread::Connection::recv(recv_queue::queue_type &o)
{
    o.clear();
    while(true)
    {
        char buffer[32 * 1024];
        int iBytesReceived = 0;
        if(_lfd == -1)  // udp
        {
            iBytesReceived = _sock.recvfrom((void*)buffer,sizeof(buffer), _ip, _port, 0);
        }
        else    // tcp
        {
            iBytesReceived = ::read(_sock.getfd(), (void*)buffer, sizeof(buffer));
        }

        if (iBytesReceived <= 0)
        {
            if(errno == EAGAIN) //没有数据了
                break;
            else    //客户端主动关闭
                return -1;
        }

        //保存接收到数据 udp
        if(_lfd == -1)
        {
            _recvbuffer.append(buffer, iBytesReceived);
        }
        else
        {
            _recvbuffer.append(buffer, iBytesReceived);
        }

        //UDP协议
        if(_lfd == -1)
        {
            if(_pBindAdapter->isIpAllow(_ip) == true)
            {
                 parseProtocol(o);  // udp数据解析
            }
            _recvbuffer = "";
        }
        // tcp
        else
        {
            //接收到数据不超过buffer,没有数据了(如果有数据,内核会再通知你 epoll et模式)
            if((size_t)iBytesReceived < sizeof(buffer))
            {
                break;
            }
            //字符串太长时substr性能会急剧下降
            if(_recvbuffer.length() > 8192)
            {
                parseProtocol(o);   // tcp数据解析
            }
        }
    }
    // tcp
    if(_lfd != -1)
    {
        return parseProtocol(o);
    }
    return o.size();
}
```

### 解析协议 ###
```
int TC_EpollServer::NetThread::Connection::parseProtocol(recv_queue::queue_type &o)
{
    try
    {
        while (true)
        {
            //需要过滤首包包头
            if(_iHeaderLen > 0)
            {
                if(_recvbuffer.length() >= (unsigned) _iHeaderLen)
                {
                    string header = _recvbuffer.substr(0, _iHeaderLen);
                    // 包头过滤函数
                    _pBindAdapter->getHeaderFilterFunctor()((int)(TC_EpollServer::PACKET_FULL), header);
                    _recvbuffer = _recvbuffer.substr(_iHeaderLen);
                    _iHeaderLen = 0;
                }
                else
                {
                    _pBindAdapter->getHeaderFilterFunctor()((int)(TC_EpollServer::PACKET_LESS), _recvbuffer);
                    _iHeaderLen -= _recvbuffer.length();
                    _recvbuffer = "";   // 清空接收缓冲
                    break;
                }
            }

            // 接收到的出去包头以外的包内容
            string ro;
            std::string* rbuf = &_recvbuffer;
            // 当前收包状态
            int b = TC_EpollServer::PACKET_LESS;
            // 非tars协议 server启动时设置协议解析器
            if (_pBindAdapter->getConnProtocol())
            {
                b = _pBindAdapter->getConnProtocol()(*rbuf, ro, this);
            }
            // 默认tars协议解析器
            else
            {
                b = _pBindAdapter->getProtocol()(*rbuf, ro);
            }

            // 包不完整
            if(b == TC_EpollServer::PACKET_LESS)
            {
                break;
            }
            // 完整的包
            else if(b == TC_EpollServer::PACKET_FULL)
            {
                // 鉴权
                if (_pBindAdapter->_authWrapper &&
                    _pBindAdapter->_authWrapper(this, ro))
                    continue;

                // 构造recv数据对象
                tagRecvData* recv = new tagRecvData();
                recv->buffer           = std::move(ro);
                recv->ip               = _ip;
                recv->port             = _port;
                recv->recvTimeStamp    = TNOWMS;
                recv->uid              = getId();
                recv->isOverload       = false;
                recv->isClosed         = false;
                recv->fd               = getfd();
                this->_bEmptyConn      = false;

                //收到完整包
                o.push_back(recv);

                // 数据超过临时队列的大小  重新将数据添加到接收数据队列 重置o对象
                if((int) o.size() > _iMaxTemQueueSize)
                {
                    insertRecvQueue(o);
                    o.clear();
                }

                if(rbuf->empty())
                    break;
            }
            else
            {
                return -1;                      //协议解析错误
            }
        }
    }
    return o.size();
}
```

* tars协议解析器
```
    template<tars::Int32 iMaxLength>
    static int parseLen(string &in, string &out)
    {
        if(in.length() < sizeof(tars::Int32))
        {
            return TC_EpollServer::PACKET_LESS;
        }

        // 获取包头指示的包长度
        tars::Int32 iHeaderLen;
        memcpy(&iHeaderLen, in.c_str(), sizeof(tars::Int32));
        iHeaderLen = ntohl(iHeaderLen);

        // 包状态判断
        if(iHeaderLen < tars::Int32(sizeof(tars::Int32))|| iHeaderLen > iMaxLength)
        {
            return TC_EpollServer::PACKET_ERR;
        }
        if((int)in.length() < iHeaderLen)
        {
            return TC_EpollServer::PACKET_LESS;
        }

        // 截取除包头以外的包内容
        out = in.substr(sizeof(tars::Int32), iHeaderLen - sizeof(tars::Int32));
        // 跳过已经读取的缓冲区内容
        in  = in.substr(iHeaderLen);
        return TC_EpollServer::PACKET_FULL;
    }
```
### 业务层消息处理 ###
```
void ServantHandle::run()
{
    initialize();
    if(!ServerConfig::OpenCoroutine)
    {
        handleImp();
    }
}
```

业务消息处理主循环
```
void TC_EpollServer::Handle::handleImp()
{
    // 开启未存活的线程
    startHandle();

    while (!getEpollServer()->isTerminate())
    {
        tagRecvData* recv = NULL;
        map<string, BindAdapterPtr>& adapters = _handleGroup->adapters;
        for (auto& kv : adapters)
        {
            BindAdapterPtr& adapter = kv.second;
            try
            {
                while (adapter->waitForRecvQueue(recv, 0))
                {
                    //上报心跳
                    heartbeat();

                    tagRecvData& stRecvData = *recv;
                    stRecvData.adapter = adapter;
                    // 数据已超载 overload
                    if (stRecvData.isOverload)
                    {
                        handleOverload(stRecvData);
                    }
                    // 关闭连接的通知消息
                    else if (stRecvData.isClosed)
                    {
                        handleClose(stRecvData);
                    }
                    // 数据在队列中已经超时了
                    else if ((now - stRecvData.recvTimeStamp) > (int64_t)adapter->getQueueTimeout())
                    {
                        handleTimeout(stRecvData);
                    }
                    // 业务数据处理
                    else
                    {
                        handle(stRecvData);
                    }
                    delete recv;
                    recv = NULL;
                }
            }
        }
    }
    stopHandle();
}

```

按协议处理消息
```
void ServantHandle::handle(const TC_EpollServer::tagRecvData &stRecvData)
{
    TarsCurrentPtr current = createCurrent(stRecvData);
    if (!current) return;

    if (current->getBindAdapter()->isTarsProtocol())
    {
        handleTarsProtocol(current);
    }
    else
    {
        handleNoTarsProtocol(current);
    }
}
```

消息分发
```
void ServantHandle::handleTarsProtocol(const TarsCurrentPtr &current)
{
    map<string, ServantPtr>::iterator sit = _servants.find(current->getServantName());
    vector<char> buffer;
    //业务逻辑处理
    ret = sit->second->dispatch(current, buffer);

    //单向调用或者业务不需要同步返回
    if (current->isResponse())
    {
        current->sendResponse(ret, buffer, TarsCurrent::TARS_STATUS(), sResultDesc);
    }
}

```

```
int Servant::dispatch(TarsCurrentPtr current, vector<char> &buffer)
{
    int ret = TARSSERVERUNKNOWNERR;
    if (current->getFuncName() == "tars_ping")
    {
        ret = TARSSERVERSUCCESS;
    }
    else if (!current->getBindAdapter()->isTarsProtocol())
    {
        TC_LockT<TC_ThreadRecMutex> lock(*this);
        ret = doRequest(current, buffer);
    }
    else
    {
        TC_LockT<TC_ThreadRecMutex> lock(*this);
        ret = onDispatch(current, buffer);
    }
    return ret;
}
```

tars协议自定义分发函数
tars协议文件由工具编译生成后自动生成分发处理代码
```
virtual int onDispatch(TarsCurrentPtr current, vector<char> &buffer) { return -1; }
```

非tars协议需要自己手动构造请求自己实现
```
virtual int doRequest(TarsCurrentPtr current, vector<char> &buffer) { return -1; }
```