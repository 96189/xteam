#include <queue>
#include <iostream>

using namespace std;

typedef void (*Pfunc)(void);

class Timer
{
public:
    int time_;
    int id_;
    Pfunc cb_;
public:
    Timer()
    {
        time_ = 0;
        id_ = 0;
        cb_ = NULL;
    }
    Timer(int time, int id, Pfunc cb)
        : time_(time), id_(id), cb_(cb)
    {

    }
    ~Timer()
    {

    }
    bool operator>(const Timer& rhs)
    {
        return this->time_ > rhs.time_;
    }
};

void callback()
{
    std::cout << "\n" ;
}

void print_queue(std::priority_queue<Timer*, vector<Timer*>, greater<Timer*> >& q)
{
    while (!q.empty())
    {
        Timer* pTimer = q.top();
        std::cout << pTimer->time_ ;// << "/" << pTimer->id_ << "/" << pTimer->cb_ << " ";
        q.pop();
    }
    std::cout << '\n';
}
template<typename T> void print_queue_(T& q) {
    while(!q.empty()) {
        std::cout << q.top() << " ";
        q.pop();
    }
    std::cout << '\n';
}
int main(int argc, char* argv[])
{
    // std::priority_queue<Timer*, vector<Timer*>, greater<Timer*> > tq;
    // for (int i = 0; i < 6; ++i)
    // {
    //     Timer *pTimer = new Timer(i, i, callback);
    //     tq.push(pTimer);
    // }
    // print_queue(tq);
    std::priority_queue<int, std::vector<int>, std::greater<int> > q2;
 
    for (int i = 0; i < 6; ++i)
        q2.push(i);
 
    print_queue_(q2);
    return 0;
}