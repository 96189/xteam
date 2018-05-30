#include <map>
#include <netinet/in.h>
#include <memory>
#include <vector>

#define BUFF_SIZE 128

class TimerObj;
// 连接数据
class client_data
{
  public:
    std::vector<char> buf_;
    int sockfd_;
    sockaddr_in addr;
    TimerObj *timer_;

  public:
    client_data();
    ~client_data();
    client_data(int fd, sockaddr_in address);
    void SetTimer(TimerObj *pobj);
};

typedef std::map<int, client_data *> ClientMap;
