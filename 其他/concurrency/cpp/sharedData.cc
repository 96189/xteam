
#include <list>
#include <mutex>
#include <algorithm>

namespace shared
{

std::list<int> some_list;
std::mutex some_mutex;

void add_to_list(int new_value)
{
    std::lock_guard<std::mutex> guard(some_mutex);
    some_list.push_back(new_value);
}

bool list_contains(int value_to_find)
{
    std::lock_guard<std::mutex> guard(some_mutex);
    return std::find(some_list.begin(), some_list.end(), value_to_find) != some_list.end();
}

// 错误示范
// 受保护的数据的指针或引用被传递到互斥锁作用域之外 导致保护失效

class some_data
{
    int a_;
    std::string b_;
public:
    void do_something()
    {
        // 读写数据
    }
}

class data_wrapper
{
private:
    some_data data_;    // 被保护的数据
    std::mutex m_;      // 互斥量
public:
    template<typename Function>
    void process_data(Function func)
    {
        std::lock_guard<std::mutex> l(m_);
        func(data_);    // 传递被保护的数据给用户函数 造成保护失效 用户函数可能是以引用传递
    }
};

some_data *unprotected;
void malicious_function(some_data& protected_data)
{
    unprotected = &protected_data;
}

data_wrapper x;
void foo()
{
    x.process_data(malicious_function); // 传递恶意的函数 取得了被保护数据的引用
    unprotected->do_something();    // 在无保护的情况下访问数据
}
};


int main(int argc, char *argv[])
{

    return 0;
}