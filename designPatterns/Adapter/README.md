adaptee 被适配对象,已存在的接口
adapter 适配器类
target  客户端使用的接口

适配器模式主要承担接口转换功能,通过adapter将adaptee的接口转换成target的接口供client使用.

类适配器模式
    adapter通过多重继承adaptee和target,继承target是为了统一类型面向接口编程,继承adaptee是为了使用它内部的方法,client通过adapter调用target的接口,接口内部实现为调用adaptee的接口.
对象适配器模式
    adapter通过内部持有adaptee实例对象,继承target.持有adaptee实例对象是为了通过对象调用内部的方法,继承target是为了统一类型面向接口编程,client通过adapter内部持有的adaptee对象调用target接口.

两种方式都需要继承target是为了统一类型,面向接口编程.
类适配器模式继承adaptee,可以重定义adaptee的部分行为(虚函数重写).由于继承无法适配adaptee的子类.
对象适配器模式组合adaptee,无法重定义adaptee的行为,由于组合可以继续适配adaptee的子类

https://www.cnblogs.com/winkey4986/p/5482064.html

