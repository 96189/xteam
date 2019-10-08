
#include <random>
#include <iostream>
#include <memory>
#include <functional>

namespace TEST
{
namespace BIND
{
void f(int n1, int n2, int n3, int n4, int n5)
{
    std::cout << n1 << " " << n2 << " " << n3 << " " << n4 << " " << n5 << std::endl;
}
int g(int n1)
{
    return n1+1;
}
struct Foo
{
    void print_sum(int n1, int n2)
    {
        std::cout << n1+n2 << std::endl;
    }
    int data = 10;
};

void test()
{
    using namespace std::placeholders;
    int n = 7;
    auto f1 = std::bind(f, _3, _1, 42, std::cref(n), n);    // 占位符表示调用时实参列表的第几个参数来替换占位符位置的形参 _3表示实参列表的第三个参数替换
    n = 10;
    // f1(1, 2, 1001);     // 输出1001 1 42 10 7

    auto f2 = std::bind(f, _3, std::bind(g, _3), _3, 4, 5);
    // f2(10, 11, 12);     // 12 13 12 4 5

    Foo foo;
    auto f3 = std::bind(&Foo::print_sum, &foo, 95, _1);
    // f3(5);  // 95+5

    // 绑定指向数据成员指针
    auto f4 = std::bind(&Foo::data, _1);
    // std::cout << f4(foo) << std::endl;  // 10

    // 常见使用情况 以分布绑定随机数生成器
    std::default_random_engine e;
    std::uniform_int_distribution<> d(0, 10);
    std::function<int()> rnd = std::bind(d, e);
    for (int n = 0; n < 10; ++n)
    {
        std::cout << rnd() << " ";
    }
    std::cout << std::endl;
}
};

namespace MEM_FN
{
struct Foo
{
    void display_greeting()
    {
        std::cout << "Hello World" << std::endl;
    }
    void display_number(int i)
    {
        std::cout << "number:" << i << std::endl;
    }
    int data = 7;
};

void test()
{
    Foo f;
    // auto greet = std::mem_fn(&Foo::display_greeting);
    // greet(&f);  // this

    // auto print_num = std::mem_fn(&Foo::display_number);
    // print_num(&f, 20); // this 参数

    // 绑定成员数据
    auto access_data = std::mem_fn(&Foo::data);
    std::cout << "data:" << access_data(&f) << std::endl;
}
};
};

int main(int argc, char *argv[])
{
    // TEST::BIND::test();
    TEST::MEM_FN::test();
    return 0;
}