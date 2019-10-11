
##   智能指针

    自动管理资源(内存、文件、连接)的生命周期,对原始资源封装一层,通过RAII利用构造和析构函数自动申请和释放资源.需要注意的是拷贝构造函数与赋值运算符的重载函数的实现,是要public还是private,这决定了资源的所有权是否可以转移以及可能导致的重复释放及空悬指针问题.但明显的一点是对于资源不应该依赖编译器默认生成的拷贝构造和赋值运算符重载函数.

+   [auto_ptr](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/4.8.2/backward/auto_ptr.h)
        
    由于可进行资源所有权的转移,所以拷贝构造函数与赋值运算符重载函数设置为public,转移意味着解除旧的对象与资源的绑定关系,由新的对象来管理资源,旧的对象不再可用,一旦再次误用旧的对象可能导致空指针解引用.此外若误用将两个智能指针管理同一个资源,其中一个释放将导致另一个为空悬指针.由于资源所有权的转移特性使得不满足容器性质,不能存放在容器中.不能用来管理一组资源,由于释放资源的时候使用的是delete而不是delete[].

    造成如上问题的大部分原因在于所有权转移的特性,使得拷贝构造函数和赋值运算符重载实现为public造成资源管理的困难.

+   [unique_ptr](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/4.8.2/bits/unique_ptr.h)

    资源所有权的唯一性,同一时刻只有一个对象可以管理资源.类实现时将拷贝构造函数和赋值运算符重载函数标记为delete,禁止资源所有权转移.引入右值引用和完美转发,实现移动构造函数和移动赋值运算符重载.unique_ptr实现了可管理数组资源的版本

    如果误用,同一时刻将两个智能指针对象管理同一个资源,unique_ptr依然会导致程序异常.

+   [shared_ptr](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/4.8.2/bits/shared_ptr_base.h)

    共享资源声明周期,引入__shared_count

+   [weak_ptr](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/4.8.2/bits/shared_ptr_base.h)

    观察资源是否已被释放,引入__weak_count

+   boost::scoped_ptr

##  Q&A
+   模板类中可以使用虚函数吗?模板成员函数可以是虚函数吗?

## 相关资料
+   [C/C++ 开源库及示例代码](https://github.com/programthink/opensource/blob/master/libs/cpp.wiki)
+   [孟岩谈《C++程序设计原理与实践》](https://blog.csdn.net/hzbooks/article/details/5767169)
+   [C++0X的三件好东西（零）](https://blog.csdn.net/myan/article/details/5877305)
+   [回复几个问题](https://blog.csdn.net/myan/article/details/5884695)
+   [function/bind的救赎（上）](https://blog.csdn.net/myan/article/details/5928531)

