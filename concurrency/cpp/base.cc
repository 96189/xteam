
#include <thread>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <functional>
#include <algorithm>

namespace Base
{

namespace Start
{
void do_some_work()
{
    std::cout << "do_some_work" << std::endl;
}
class background_task
{
public:
    void operator()() const 
    {
        std::cout << "background_task" << std::endl;
    }
};
void Test()
{
    std::thread t1(do_some_work);
    t1.join();

    // background_task f;
    // std::thread t2(f);
    // std::thread t2(background_task());   // error
    // std::thread t2((background_task()));
    // std::thread t2{background_task()};
    std::thread t2([]{
        std::cout << "background_task" << std::endl;
    });
    t2.join();
}
};

namespace End
{
struct func
{
    int& i_;
    func(int i) : i_(i) {}
    ~func()
    {
        i_ = -1;
    }
    void operator()()
    {
        sleep(1);
        std::cout << i_ << std::endl;
    } 
};
void oopsTest()
{
    int some_local_state = 0;
    func myFunc(some_local_state);
    std::thread myThread(myFunc);
    myThread.detach();  // detach潜在的问题
    // myThread error
}
void joinTest()
{
    int some_local_state = 0;
    func myFunc(some_local_state);
    std::thread t(myFunc);
    try 
    {
        //
    }
    catch (...)
    {
        t.join();   // 防止正常的join由于异常无法执行
        throw;
    }
    t.join();
}

class thread_guard
{
    std::thread& t_;
public:
    explicit thread_guard(std::thread& t) : t_(t) {}
    ~thread_guard()
    {
        if (t_.joinable())
        {
            std::cout << "wait thread" << std::endl;
            t_.join();
        }
    }
    thread_guard(const thread_guard &) = delete;
    thread_guard& operator=(const thread_guard&) = delete;
};
void threadRaii()
{
    int some_local_state = 0;
    func myFunc(some_local_state);
    std::thread t(myFunc);
    thread_guard g(t);
    // do some thing
}
};

namespace Move
{
void some_function()
{

}
void some_other_function()
{

}
void Test()
{
    std::thread t1(some_function);
    std::thread t2 = std::move(t1);
    t1 = std::thread(some_other_function);
    std::thread t3;
    t3 = std::move(t2);
    // t1 = std::move(t3); error
    t1.join();
    t3.join();
}

class scoped_thread
{
    std::thread t_;
public:
    explicit scoped_thread(std::thread t) 
    : t_(std::move(t)) 
    {
        std::cout << "scoped_thread" << std::endl;
        if (!t_.joinable())
        {
            std::cout << "ERROR no thread" << std::endl;
        }
    }
    ~scoped_thread()
    {
        std::cout << "~scoped_thread" << std::endl;
        std::cout << "wait thread" << std::endl;
        t_.join();
    }
    scoped_thread(const scoped_thread&) = delete;
    scoped_thread& operator=(const scoped_thread&) = delete;
};
struct func
{
    int& i_;
    func(int i) : i_(i) {}
    ~func()
    {
        i_ = -1;
    }
    void operator()()
    {
        std::cout << i_ << std::endl;
    } 
};
void scopedTest()
{
    int some_local_state;

    // scoped_thread t(std::thread(func(some_local_state)));       // 无效
    // scoped_thread t(std::thread([] 
    // {
    //     // std::cout << std::this_thread::get_id() << std::endl; 
    // }));
    // scoped_thread t((std::thread(func(some_local_state))));
    scoped_thread t{std::thread(func(some_local_state))};
}
void do_worker(int i)
{
    std::cout << "thread num " << i << std::endl;
}
void threads()
{
    std::vector<std::thread> threadVec;
    for (size_t i = 0; i < 10; ++i)
    {
        threadVec.push_back(std::thread(do_worker, i));
    }
    // std::bind 绑定一或多个参数到函数对象 
    // std::mem_fn 从指向成员指针创建出函数对象
    // 为集合中的每个线程对象施加join操作
    std::for_each(threadVec.begin(), threadVec.end(), std::mem_fn(&std::thread::join));
}
};

namespace parallel
{
template <typename Iterator, typename T>
struct accumulate_block
{
    void operator()(Iterator first, Iterator last, T &result)
    {
        result = std::accumulate(first, last, result);
        std::cout << std::this_thread::get_id() << " -> " << result << std::endl;
    }
};

template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    unsigned long const length = std::distance(first, last);

    if (!length) // 如果是空序列 则返回给定的sum初值
        return init;

    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread; // 2

    unsigned long const hardware_threads = std::thread::hardware_concurrency(); // 获取硬件支持的线程数

    // unsigned long const num_threads = // 
    unsigned long num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    // TODO FOR TEST
    if (num_threads == 1)
    {
        num_threads = 4;
    }

    unsigned long const block_size = length / num_threads; // 计算每一个线程处理的数据块大小

    std::vector<T> results(num_threads);    // 结果集 每一个线程对应一个结果
    std::vector<std::thread> threads(num_threads - 1); // 线程集合

    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size); // 数据分块
        // 生成线程 指定每个线程处理的数据的部分
        threads[i] = std::thread(accumulate_block<Iterator, T>(), block_start, block_end, std::ref(results[i]));
        block_start = block_end; // 调整块偏移
    }

    accumulate_block<Iterator, T>()(block_start, last, results[num_threads - 1]); // 主线程执行同样的计算算法
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join)); // 主线程等待每个线程结束

    return std::accumulate(results.begin(), results.end(), init); // 主线程汇总各个线程的执行结果
}
void runtimeParallelThreadTest()
{
    std::vector<int> array = {1,2,3,4,5,6,7,8,9};
    // 并行算法计算 每个线程执行算法的一部分 主线程将最后的结果汇总
    // 算法功能:对array序列中的值求和sum sum初始值为0
    int val = parallel_accumulate<std::vector<int>::iterator, int>(array.begin(), array.end(), 0);
    std::cout << "parallel_accumulate : " << val << std::endl;
}
};

};


int main(int argc, char* argv[])
{
    // Base::Start::Test();
    // Base::End::oopsTest();
    // Base::End::threadRaii();
    // Base::Move::Test();
    // Base::Move::scopedTest();
    // Base::Move::threads();
    Base::parallel::runtimeParallelThreadTest();
    return 0;
}

