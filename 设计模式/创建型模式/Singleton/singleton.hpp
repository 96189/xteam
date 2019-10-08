
//

#ifndef _SINGLETON_HPP_
#define _SINGLETON_HPP_

#include <cstddef>

// 延迟加载版本
// class Singleton 
// {
// private:
//     static Singleton* uniqueInstance_;   // 资源无法释放

// private:
//     Singleton()
//     {

//     }
//     Singleton(const Singleton&);
//     Singleton& operator=(const Singleton&);
//     ~Singleton();
// public:
//     static Singleton* getInstance()
//     {
//         // 此处的判断 在多线程情况下 会造成问题 可能产生多份实例 
//         // 解决办法是加锁 但是加锁存在性能问题
//         // 先判断 若为空再加锁 加完锁 再判断 可以缓解上述性能问题 但是并不能根本解决
//         if (uniqueInstance_ == NULL)    
//         {
//             uniqueInstance_ = new Singleton();  // 延迟实例化 不调用本方法不会产生对象
//         }
//         return uniqueInstance_;
//     }
// };
// Singleton* Singleton::uniqueInstance_ = NULL;


// class Singleton
// {
// private:
//     static Singleton instance_;     // 静态类成员变量 程序结束自动释放资源

// private:
//     Singleton(const Singleton&);
//     Singleton& operator=(const Singleton&);
//     ~Singleton();
    
// public:
//     static Singleton& getInstance()     // 即使未调用本方法 对象依然会被加载
//     {
//         return instance_;
//     }
// };

class Singleton 
{
private:
    Singleton()
    {

    }
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);     // 防拷贝
    ~Singleton();
public:
    static Singleton& getInstance()
    {
        static Singleton instance;  // 静态局部变量 编译器保证只生成一份实例 加载时才生成 依赖编译器
        return instance;
    }
};

// boost ?
// class Singleton
// {
// private:
//     struct Proxy 
//     {
//         Proxy()
//         {
//             Singleton::getInstance();
//         }
//     };

// private:
//     static Proxy proxy_;
//     Singleton(const Singleton&);
//     Singleton& operator=(const Singleton&);
//     ~Singleton();

// public:
//     static Singleton& getInstance()
//     {
//         static Singleton instance;
//         return instance;
//     }    
// };

// 线程安全性由pthread_once保证
namespace muduo
{

namespace detail
{
// This doesn't detect inherited member functions!
// http://stackoverflow.com/questions/1966362/sfinae-to-check-for-inherited-member-functions
template <typename T>
struct has_no_destroy
{
    template <typename C>
    static char test(decltype(&C::no_destroy));
    template <typename C>
    static int32_t test(...);
    const static bool value = sizeof(test<T>(0)) == 1;
};
} // namespace detail

template <typename T>
class Singleton : noncopyable
{
public:
    Singleton() = delete;
    ~Singleton() = delete;

    static T &instance()
    {
        pthread_once(&ponce_, &Singleton::init);
        assert(value_ != NULL);
        return *value_;
    }

private:
    static void init()
    {
        value_ = new T();
        if (!detail::has_no_destroy<T>::value)
        {
            ::atexit(destroy);
        }
    }

    static void destroy()
    {
        typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
        T_must_be_complete_type dummy;
        (void)dummy;

        delete value_;
        value_ = NULL;
    }

private:
    static pthread_once_t ponce_;
    static T *value_;
};

template <typename T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template <typename T>
T *Singleton<T>::value_ = NULL;

} // namespace muduo

#endif // MUDUO_BASE_SINGLETON_H

#endif