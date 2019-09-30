
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



#endif