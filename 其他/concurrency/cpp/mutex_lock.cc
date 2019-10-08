
#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <functional>
#include <chrono>
#include <string>

namespace Test
{

namespace ScopedLock
{
struct Employee
{
    Employee(std::string id) : id_(id) {}
    std::string output() const 
    {
        std::string ret = "Employee " + id_ + " has lunch partners: ";
        for (const auto& partner : lunch_partners_)
        {
            ret += partner + " ";
        }
        return ret;
    }

    std::string id_;
    std::vector<std::string> lunch_partners_;
    std::mutex m_;
};

void send_mail(Employee& e1, Employee& e2)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void assign_lunch_partner(Employee &e1, Employee &e2)
{
    static std::mutex io_mutex;
    {
        std::lock_guard<std::mutex> lk(io_mutex);
        std::cout << e1.id_ << " and " << e2.id_ << " are waiting for locks" << std::endl;
    }
    {
        // std::scoped_lock lock(e1.m_, e2.m_);    // c++17支持 
        // 锁住两个互斥量 内部免死锁算法避免死锁
        std::lock(e1.m_, e2.m_);
        // adopt_lock_t	假设调用方线程已拥有互斥的所有权
        std::lock_guard<std::mutex> lk1(e1.m_, std::adopt_lock);
        std::lock_guard<std::mutex> lk2(e2.m_, std::adopt_lock);

        {
            std::lock_guard<std::mutex> lk(io_mutex);
            std::cout << e1.id_ << " and " << e2.id_ << " got locks" << std::endl;
        }

        e1.lunch_partners_.push_back(e2.id_);
        e2.lunch_partners_.push_back(e1.id_);
    }
    send_mail(e1, e2);
    send_mail(e2, e1);
}

void test()
{
    Employee alice("alice"), bob("bob"), christina("christina"), dave("dave");

    std::vector<std::thread> threads;
    threads.emplace_back(assign_lunch_partner, std::ref(alice), std::ref(bob));
    threads.emplace_back(assign_lunch_partner, std::ref(christina), std::ref(bob));
    threads.emplace_back(assign_lunch_partner, std::ref(christina), std::ref(alice));
    threads.emplace_back(assign_lunch_partner, std::ref(dave), std::ref(bob));

    for (auto &thread : threads) thread.join();

    std::cout << alice.output() << '\n' << bob.output() << '\n' << christina.output() << '\n' << dave.output() << std::endl;
}

};


namespace UniqueLock
{
struct Box
{
    explicit Box(int num) : numThings_(num) {}

    int numThings_;
    std::mutex m_;
};

void transfer(Box& from, Box& to, int num)
{
    // 仍未实际获取锁
    // defer_lock_t	不获得互斥的所有权
    std::unique_lock<std::mutex> lock1(from.m_, std::defer_lock);
    std::unique_lock<std::mutex> lock2(to.m_, std::defer_lock);

    // 锁两个两个互斥量 内部免死锁算法避免死锁
    std::lock(lock1, lock2);

    from.numThings_ -= num;
    to.numThings_ += num;
}

void test()
{
    Box acc1(100);
    Box acc2(50);

    std::thread t1(transfer, std::ref(acc1), std::ref(acc2), 10);
    std::thread t2(transfer, std::ref(acc2), std::ref(acc1), 5);

    t1.join();
    t2.join();
}
};

namespace InitOnce
{

// std::call_once 多线程调用 函数只执行一次 用在资源初始化一次的场景
std::once_flag flag1, flag2;
void simple_do_once()
{
    std::call_once(flag1, [](){ 
        std::cout << "Simple example: called once" << std::endl; 
    });
}

void may_throw_function(bool do_throw)
{
    if (do_throw)
    {
        std::cout << "throw: call_once will retry" << std::endl;
        throw std::exception();
    }
    std::cout << "did not throw, call_once will not attempt again" << std::endl;
}

void do_once(bool do_throw)
{
    try 
    {
        std::call_once(flag2, may_throw_function, do_throw);
    }
    catch (...)
    {

    }
}

void test()
{
    // std::thread st1(simple_do_once);
    // std::thread st2(simple_do_once);
    // std::thread st3(simple_do_once);
    // std::thread st4(simple_do_once);
    // st1.join();
    // st2.join();
    // st3.join();
    // st4.join();

    std::thread t1(do_once, true);
    std::thread t2(do_once, true);
    std::thread t3(do_once, false);
    std::thread t4(do_once, true);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
};

};


int main(int argc, char *argv[])
{
    // Test::ScopedLock::test();
    // Test::UniqueLock::test();
    Test::InitOnce::test();
    return 0;
}