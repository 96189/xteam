#include "server.h"

int split_str(const char *ps_str, char *ps_sp, std::vector<std::string> &v_ret)
{
    char *ps_temp;
    char *p;
    int i_len = (int)strlen(ps_str);
    std::string st_str;
    ps_temp = new char[i_len + 2];
    snprintf(ps_temp, i_len + 1, "%s", ps_str);
    char *last = NULL;
    p = strtok_r(ps_temp, ps_sp, &last);
    if (NULL == p)
    {
        delete ps_temp;
        return 0;
    }
    st_str = (std::string)p;
    v_ret.push_back(st_str);
    while (NULL != (p = strtok_r(NULL, ps_sp, &last)))
    {
        st_str = (std::string)p;
        v_ret.push_back(st_str);
    }
    delete ps_temp;
    return 0;
}

// 非阻塞
int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void EnableInput(uint32_t &events)
{
    events |= EPOLLIN;
}
void EnableOutput(uint32_t &events)
{
    events |= EPOLLOUT;
}
void DisableInput(uint32_t &events)
{
    events &= ~EPOLLIN;
}
void DisableOutput(uint32_t &events)
{
    events &= ~EPOLLOUT;
}

// add del modify
void UpdateEvents(int epfd, int fd, int event, int op)
{
    struct epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;
    epoll_ctl(epfd, op, fd, &ev);
}
// 从监听集合中删除fd
// 释放该fd资源
void cb_func(int sockfd)
{
    UpdateEvents(g_epollfd, sockfd, 0, EPOLL_CTL_DEL);
    printf("close fd %d\n", sockfd);
    printf("delete connfd:%d timer:0x%p\n", sockfd, g_users_map[sockfd]->timer_);
    close(sockfd);
    g_users_map.erase(sockfd);
}
// io事件和定时器事件统一处理
int UnifyEvent(int g_epollfd, epoll_event *events, int max_event_num)
{
    int nEvents;
    time_t delta;
    time_t expire_time = 0;
    while (true)
    {
        if (!g_timer_manager.Empty())
        {
            // ms
            expire_time = g_timer_manager.GetMinExpireTime();
            time_t curtime = (GET_TIMESTAMP() / 1000);
            // 计算相对事件 单位ms
            delta = expire_time - curtime;
            if (delta < 0)
            {
                printf("delta:%ld expire_time:%ld curtime:%ld\n", delta, expire_time, curtime);
                delta = -1;
            }
        }
        else
        {
            delta = -1;
        }
        // ms
        nEvents = epoll_wait(g_epollfd, events, max_event_num, delta);
        // 无事件就绪,但超时
        if (0 == nEvents)
        {
            g_timer_manager.CheckExpired(expire_time);
        }
        else
        {
            break;
        }
    }
    return nEvents;
}
void SetConnData2Cache(int conn, char *buffer)
{
    g_users_map[conn]->buf_.assign(buffer, buffer + strlen(buffer));
}
void SendCacheData2Conn(int conn)
{
    send(conn, &g_users_map[conn]->buf_[0], g_users_map[conn]->buf_.size(), 0);
}
void ResetConnCache(int conn)
{
    g_users_map[conn]->buf_.clear();
}
// 新连接建立
void ProcAccept(int listenfd)
{
    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);
    int connfd = accept(listenfd, (struct sockaddr *)&client_address, &client_addrlength);
    assert(connfd > -1);
    setnonblocking(connfd);
    UpdateEvents(g_epollfd, connfd, EPOLLIN, EPOLL_CTL_ADD);

    client_data *data = new client_data(connfd, client_address);
    g_users_map[connfd] = data;
    time_t delta = TIMEOUT;
    TimerObj *timer = new TimerObj(delta);
    timer->SetTimeOutCallBack(cb_func, connfd);
    g_users_map[connfd]->SetTimer(timer);
    g_timer_manager.StartTimer(timer);
    printf("new add connfd:%d timer:0x%p\n", connfd, g_users_map[connfd]->timer_);
}

// 已建立的连接消息到达(业务无关代码)
void ProcRead(int sockfd)
{
    char buffer[BUFF_SIZE] = {};
    int ret = recv(sockfd, buffer, BUFF_SIZE - 1, 0);

    printf("get %d bytes of client data %s from %d\n", ret, buffer, sockfd);
    TimerObj *timer = g_users_map[sockfd]->timer_;
    if (ret < 0)
    {
        if (errno != EAGAIN)
        {
            cb_func(sockfd);
            if (timer)
            {
                g_timer_manager.StopTimer(timer);
            }
        }
    }
    else if (ret == 0)
    {
        cb_func(sockfd);
        if (timer)
        {
            g_timer_manager.StopTimer(timer);
        }
    }
    else
    {
        time_t delta = TIMEOUT;

        if (g_option.transfile)
        {
            delta = TIMEOUT * 5;
            TransFile(sockfd, buffer);
        }
        else
        {
            delta = TIMEOUT;
            if (g_option.echo)
            {
                Echo(sockfd, buffer);
            }
            else if (g_option.chat)
            {
                Chat(sockfd, buffer);
            }
        }

        if (timer)
        {
            printf("restart connfd:%d timer:0x%p\n", sockfd, g_users_map[sockfd]->timer_);
            g_timer_manager.StopTimer(timer);

            TimerObj *newtimer = new TimerObj(delta);
            newtimer->SetTimeOutCallBack(cb_func, sockfd);
            g_users_map[sockfd]->SetTimer(newtimer);
            g_timer_manager.StartTimer(newtimer);
            printf("restart connfd:%d newtimer:0x%p\n", sockfd, g_users_map[sockfd]->timer_);
        }
    }
}
// 可读事件处理
void HandleInput(int sockfd, uint32_t events)
{
    // 新连接建立
    if (sockfd == g_listenfd)
    {
        ProcAccept(g_listenfd);
    }
    // 已有连接数据到达
    else
    {
        ProcRead(sockfd);
    }
}

// 可写事件处理
void HandleOutput(int sockfd, uint32_t events)
{
    if (g_users_map[sockfd]->buf_.size() > 0)
    {
        SendCacheData2Conn(sockfd);
    }

    DisableOutput(events);
    EnableInput(events);
    UpdateEvents(g_epollfd, sockfd, events, EPOLL_CTL_MOD);
}

// 业务代码
void Echo(int sockfd, char *buffer)
{
    SetConnData2Cache(sockfd, buffer);

    uint32_t events = 0;
    DisableInput(events);
    EnableOutput(events);
    UpdateEvents(g_epollfd, sockfd, events, EPOLL_CTL_MOD);
}

void Chat(int sockfd, char *buffer)
{
    for (ClientMap::iterator it = g_users_map.begin(); it != g_users_map.end(); ++it)
    {
        if (it->first == sockfd)
        {
            continue;
        }
        SetConnData2Cache(it->first, buffer);

        uint32_t events = 0;
        DisableInput(events);
        EnableOutput(events);
        UpdateEvents(g_epollfd, it->first, events, EPOLL_CTL_MOD);
    }
}

void TransFile(int sockfd, char *buffer)
{
    buffer[strlen(buffer) - 1] = '\0';
    std::vector<std::string> req;
    split_str(buffer, " ", req);
    if (req[0] == "get")
    {
        std::string filepath = std::string("./") + req[1];
        // printf("dir/file:%s\n", filepath.c_str());
        SendFile(sockfd, filepath);
    }
    uint32_t events = 0;
    DisableInput(events);
    EnableOutput(events);
    UpdateEvents(g_epollfd, sockfd, events, EPOLL_CTL_MOD);
}

void SendFile(int connfd, std::string filename)
{
    printf("sendfile %s\n", filename.c_str());
    int filefd = open(filename.c_str(), O_RDONLY);
    if (filefd < 0)
    {
        char *mesg = strerror(errno);
        printf("Mesg:%s %s\n", mesg, filename.c_str());
        return;
    }
    const int filesize = 64 * 1024;
    char buffer[filesize] = {'\0'};
    int size = read(filefd, buffer, filesize);
    assert(size != -1);
    SetConnData2Cache(connfd, buffer);
    close(filefd);
}