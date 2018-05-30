#include "server.h"

Option g_option;

int g_epollfd = -1;
int g_listenfd = -1;

ClientMap g_users_map;
TimerManager g_timer_manager;

bool ParseCommandLine(int argc, char *argv[], const char *optString, Option *option)
{
    int opt = -1;
    int ret_code = false;
    printf("Start Server at\n");
    while ((opt = getopt(argc, argv, optString)) != -1)
    {
        ret_code = true;
        switch (opt)
        {
        // ip
        case 'h':
            strcpy(option->ip, optarg);
            printf("ip:%s\n", option->ip);
            break;
        // port
        case 'p':
            option->port = atoi(optarg);
            printf("port:%d\n", option->port);
            break;
        // transfile
        case 't':
            option->transfile = true;
            // option->filename = std::string(optarg);
            printf("transfile:%d\n", option->transfile);
            break;
        // echo
        case 'e':
            option->echo = true;
            printf("echoserver:%d\n", option->echo);
            break;
        // chat
        case 'c':
            option->chat = true;
            printf("chatserver:%d\n", option->chat);
            break;
        default:
            ret_code = false;
            break;
        }
    }
    return ret_code;
}

int main(int argc, char *argv[])
{
    const char *optString = "h:p:etc";
    memset(&g_option, 0, sizeof(g_option));
    int ret = ParseCommandLine(argc, argv, optString, &g_option);
    assert(ret);

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, g_option.ip, &address.sin_addr);
    address.sin_port = htons(g_option.port);

    g_listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(g_listenfd > 0);

    int reuse_addr = 1;
    setsockopt(g_listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));
    ret = bind(g_listenfd, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(g_listenfd, 5);
    assert(ret != -1);

    g_epollfd = epoll_create(5);
    assert(g_epollfd != -1);

    setnonblocking(g_listenfd);
    UpdateEvents(g_epollfd, g_listenfd, EPOLLIN, EPOLL_CTL_ADD);

    struct epoll_event *events = (struct epoll_event *)malloc(sizeof(struct epoll_event) * MAX_EVENT_NUM);
    bool stopserver = false;
    int nEvents;
    while (!stopserver)
    {
        nEvents = UnifyEvent(g_epollfd, events, MAX_EVENT_NUM);
        if (nEvents < 0 && errno != EINTR)
        {
            break;
        }

        for (int i = 0; i < nEvents; ++i)
        {
            if (events[i].events & EPOLLIN)
            {
                HandleInput(events[i].data.fd, events[i].events);
            }
            else if (events[i].events & EPOLLOUT)
            {
                HandleOutput(events[i].data.fd, events[i].events);
            }
            else
            {
                stopserver = true;
                break;
            }
        }
    }
    free(events);
    close(g_listenfd);
    return 0;
}