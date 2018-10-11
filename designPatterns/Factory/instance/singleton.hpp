
// 单例模式

#ifndef _SINGLETON_HPP_
#define _SINGLETON_HPP_

template <typename T>
class Singleton 
{
private:
    Singleton()
    {

    }
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);     // 防拷贝
    ~Singleton()
    {

    }
public:
    static T* Instance()
    {
        static T instance;  // 静态局部变量 编译器保证只生成一份实例 加载时才生成 依赖编译器
        return &instance;
    }
};

#endif