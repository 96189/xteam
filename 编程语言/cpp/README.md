
### [c++复习](https://leetcode-cn.com/leetbook/detail/cpp-interview-highlights/)
* 编译内存相关
    
    1. c++程序编译连接过程
    2. c++内存管理

        大内存从系统malloc申请
        小内存从mempool获取,自由链表管理
    3. 栈和堆的区别
    4. 各种类型变量与内存空间地址
    5. 全局变量定义在都文件中有什么问题
    6. 对象创建限制在栈或堆
    7. 内存对齐
    8. 类的大小
    9. 如何防止内存泄露？内存泄露检查工具的原理？
    10. 智能指针有哪几种？实现原理？
    11. 一个unique_ptr怎么赋值给另一个unique_ptr对象？
    12. 使用智能指针可能出现哪些问题？如何解决？

* 语言对比

    1. c++11新特性
        
        auto

        bind和function
        
    2. c和c++的区别
    3. python和c++的区别
    4. go和c++的区别

* 面向对象

    1. 什么是面向对象？
    2. 重载、重写、隐藏的区别
    3. 如何理解c++是面向对象编程
    4. 什么是多态？多态如何实现？

* 关键字库函数

    1. sizeof和len的区别
    2. lambda表达式(匿名函数)的具体应用和使用场景
    3. explicit 的作用（如何避免编译器进行隐式类型转换）
    4. c和c++中static的区别

        c++类成员变量static共享
    5. static的作用
    6. static再类中的使用注意事项(定义、初始化和使用)
    7. static全局变量和普通全局变量的异同
    8. const作用及用法
    9. define和const的区别
    10. define和typedef的区别
    11. 用宏实现比较大小，以及两个数中的最小值
    12. inner作用及使用方法、innner函数工作原理
    13. new的作用
    14. new和malloc如何判断是否申请到内存，底层原理
    15. delete实现原理？delete和delete []的区别
    16. new和malloc区别，delete和free区别
    17. c和c++ struct区别
    18. 为什么有了class还保留struct
    19. volatile作用？是否具有原子性，对编译器的影响
    20. 什么情况下一定要用volatile，能否和const一起使用
    21. 返回函数中的静态变量地址会发生什么
    22. extern C的作用
    23. sizeof(1==1)在C和c++中分别是什么结果
    24. memcpy函数底层原理
    25. strcpy函数有什么缺陷
    26. auto类型推导原理

* 类相关

    1. 虚函数与纯虚函数区别
    2. 虚函数实现机制
    3. 单继承和多继承虚函数表结构
    4. 如何禁止构造函数的使用
    5. 为什么编译器生成类的默认构造函数等函数
    6. 构造函数、析构函数是否需要定义为虚函数？为什么？
    7. 如何避免拷贝？
    9. 如何减少构造函数开销
    10. 多重继承会出现什么问题？如何解决？
    11. 空类占多少字节，编译器为空类自动生成哪些函数？
    12. 为什么拷贝构造函数必须为引用？
    13. c++类对象的初始化顺序
    14. 如何禁止一个类被实例化
    15. 为什么用成员初始化列表会更快
    16. 实例化一个对象需要哪些阶段
    17. 友元函数的作用和应用场景
    18. 静态绑定和动态绑定是如何实现的
    19. 深拷贝和浅拷贝的区别
    20. 编译时多态和运行时多态的区别
    21. 实现一个类成员函数，要求不允许修改类的成员变量
    22. 如何让类不能被继承

* 语言特性相关

    1. 左值和右值的区别？左值引用和右值引用的区别？如何将左值转换成右值？
    2. std::move()函数实现原理

        底层是类型转换
    3. 什么是指针？指针大小及用法？
    4. 什么是野指针和空悬指针
    5. c++11 nullptr比NULL优势
    6. 指针和引用的区别
    7. 常量指针和指针常量
    8. 函数指针和指针函数
    9. 强制类型转换有几种
    10. 如何判断结构体是否相等？能否用memcmp函数判断结构体相等？
    11. 参数传递时，值传递、指针传递、引用传递的区别
    12. 什么是模板，如何实现？
    13. 函数模板和类模板区别
    14. 可变参数模板
    15. 模板特化是什么？为什么特化？
    16. inclue ""和<>的区别
    17. switch的case里为什么不能定义变量？
    18. 迭代器的作用
    19. 泛型编程如何实现
    20. 什么是类型萃取

* STL相关

    1. vector满，底层会发生什么？
    2. 各种容器的应用场景，查询元素的时间复杂度

##   智能指针

    自动管理资源(内存、文件、连接)的生命周期,对原始资源封装一层,通过RAII利用构造和析构函数自动申请和释放资源.需要注意的是拷贝构造函数与赋值运算符的重载函数的实现,是要public还是private,这决定了资源的所有权是否可以转移以及可能导致的重复释放及空悬指针问题.但明显的一点是对于资源不应该依赖编译器默认生成的拷贝构造和赋值运算符重载函数.

+   [auto_ptr](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/4.8.2/backward/auto_ptr.h)
        
    由于可进行资源所有权的转移,所以拷贝构造函数与赋值运算符重载函数设置为public,转移意味着解除旧的对象与资源的绑定关系,由新的对象来管理资源,旧的对象不再可用,一旦再次误用旧的对象可能导致空指针解引用.此外若误用将两个智能指针管理同一个资源,其中一个释放将导致另一个为空悬指针.由于资源所有权的转移特性使得不满足容器性质,不能存放在容器中.不能用来管理一组资源,由于释放资源的时候使用的是delete而不是delete[].

    造成如上问题的大部分原因在于所有权转移的特性,使得拷贝构造函数和赋值运算符重载实现为public造成资源管理的困难.

+   [unique_ptr](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/4.8.2/bits/unique_ptr.h)

    资源独占(ownership独占)

    资源所有权的唯一性,同一时刻只有一个对象可以管理资源.类实现时将拷贝构造函数和赋值运算符重载函数标记为delete,禁止资源所有权转移.引入右值引用和完美转发,实现移动构造函数和移动赋值运算符重载.unique_ptr实现了可管理数组资源的版本

    如果误用,同一时刻将两个智能指针对象管理同一个资源,unique_ptr依然会导致程序异常.

    malloc free需要配对，中间其他代码若异常，造成内存泄露。考虑使用unique_ptr，使用make_unique(value)，即RAII

+   [shared_ptr](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/4.8.2/bits/shared_ptr_base.h)

    共享资源(ownership共享)

    共享资源声明周期,引入__shared_count

    使用make_shared(value)创建

+   [weak_ptr](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/4.8.2/bits/shared_ptr_base.h)

    观察资源是否已被释放,引入__weak_count

+   boost::scoped_ptr

## 继承
+   实现一个不能被继承的类(FinalClass)

    virtual继承的特性虚基类构造函数被优先调用一次
    
    友元特性不会被子类继承

    模板可以实现任何类型的FinalClass,否则要手动实现多份

    ```
    template <typename T>
    class Base
    {
        friend T;
    private:
        Base() {}
        ~Base() {}
    };

    class FinalClass : virtual Base<FinalClass>
    {
    public:
        FinalClass() {}
        ~FinalClass() {}
    };

    // 无法继承 
    class ErrorClass : public FinalClass
    {

    };

    int main(int argc, char *argv[]) 
    {
        FinalClass f;
        // ErrorClass e;    // 编译错误
    }
    ```

##  Q&A
+   模板类中可以使用虚函数吗?模板成员函数可以是虚函数吗?

## 相关资料
+   [C/C++ 开源库及示例代码](https://github.com/programthink/opensource/blob/master/libs/cpp.wiki)
+   [孟岩谈《C++程序设计原理与实践》](https://blog.csdn.net/hzbooks/article/details/5767169)
+   [C++0X的三件好东西（零）](https://blog.csdn.net/myan/article/details/5877305)
+   [回复几个问题](https://blog.csdn.net/myan/article/details/5884695)
+   [function/bind的救赎（上）](https://blog.csdn.net/myan/article/details/5928531)

