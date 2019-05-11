
#include <iostream>
#include <vector>
#include <memory>
#include <cstdio>
#include <fstream>
#include <cassert>
#include <functional>

namespace TEST
{


namespace UNIQUE_PTR
{
struct B
{
    virtual ~B() = default;
    virtual void bar()
    {
        std::cout << "B::bar" << std::endl;
    }
};
struct D : public B
{
    D()
    {
        std::cout << "D::D" << std::endl;
    }
    ~D()
    {
        std::cout << "D::~D" << std::endl;
    }
    void bar() override 
    {
        std::cout << "D::bar" << std::endl;
    }
};

std::unique_ptr<B> pass_through(std::unique_ptr<B> p)
{
    p->bar();
    return p;
}
void close_file(std::FILE* fp) 
{ 
    std::fclose(fp); 
    std::cout << "close file" << std::endl;
}
void test()
{
    // std::cout << "unique ownership semantics demo" << std::endl;
    // {
    //     // 使用B提供的析构函数释放对象
    //     std::unique_ptr<B> p(new D());
    //     auto q = pass_through(std::move(p));    // 所有权移动
    //     assert(!p);
    //     q->bar();
    // }   // 离开此作用域 智能指针对象析构

    // std::cout << "Runtime polymorphism demo" << std::endl;
    // {
    //     // 使用B提供的析构函数释放对象
    //     std::unique_ptr<B> p(new D());
    //     p->bar();
    //     std::vector<std::unique_ptr<B>> v;
    //     v.push_back(std::unique_ptr<B>(new D()));
    //     v.push_back(std::move(p));  // 所有权移动
    //     v.emplace_back(new D());

    //     for (auto& p : v)
    //     {
    //         p->bar();
    //     }   // 3次析构
    // }

    // unique_ptr存放资源对象
    // std::cout << "Custom deleter demo\n";
    // std::ofstream("demo.txt") << "fake content"; // 准备要读的文件
    // {
    //     // 自定义FILE对象删除器释放资源
    //     std::unique_ptr<std::FILE, void (*)(std::FILE *)> fp(std::fopen("demo.txt", "r"), close_file);
    //     if (fp) 
    //         std::cout << (char)std::fgetc(fp.get()) << '\n';
    // } // fclose() 调用于此 但仅若 FILE* 不是空指针

    // lambda表达式 自定义对象删除器
    // std::cout << "Custom lambda-expression deleter demo" << std::endl;
    // {
    //     std::unique_ptr<D, std::function<void(D*)>> p(new D, [](D *ptr){
    //         std::cout << "destroying from a custom deleter..." << std::endl;
    //         delete ptr;
    //     });
    //     p->bar();
    // }   // 使用自定义的删除器销毁对象

    std::cout << "Array from of unique_ptr demo" << std::endl;
    {
        std::unique_ptr<D[]> p(new D[3]);
    }   // 销毁对象数组
}
};


};

int main(int argc, char *argv[])
{
    TEST::UNIQUE_PTR::test();
    return 0;
}