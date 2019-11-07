
## 适配器模式 ##

+ 适配器模式用来做什么

    主要承担接口转换功能,通过adapter将adaptee的接口转换成target的接口供client使用.

+ 模式组件与结构

        adaptee 被适配对象,已存在的接口
        adapter 适配器类
        target  客户端使用的接口

+ 类适配器模式

        adapter通过多重继承adaptee和target,继承target是为了统一类型面向接口编程,继承adaptee是为了使用它内部的方法,client通过adapter调用target的接口,接口内部实现为调用adaptee的接口.

+ 对象适配器模式

        adapter通过内部持有adaptee实例对象,继承target.持有adaptee实例对象是为了通过对象调用内部的方法,继承target是为了统一类型面向接口编程,client通过adapter内部持有的adaptee对象调用target接口.

+ 对比

        两种方式都需要继承target是为了统一类型,面向接口编程.
        类适配器模式继承adaptee,可以重定义adaptee的部分行为(虚函数重写).由于继承无法适配adaptee的子类.
        对象适配器模式组合adaptee,无法重定义adaptee的行为,由于组合可以继续适配adaptee的子类

    https://www.cnblogs.com/winkey4986/p/5482064.html

## 装饰器模式 ## 

+ 装饰器模式用来做什么

        为真实对象扩展功能(不使用继承),通过decorator持有component的实例对象componentConcrete的引用,在decoratorConcrete中组合行为,
    
        decoratorConcrete构造函数的参数是component引用,decoratorConcrete对象内部保存旧的行为,然后在类方法中实现新的行为并与旧的行为组合。

```
                component   <--->   client       // componentConcrete和decorator实现相同的接口是为了client可以不加区分的使用真实对象和装饰对象的接口
                |       |                        // 继承是为了统一类型 装饰对象可以取代真实对象
componentConcrete       decorator
                            |
                            decoratorConcrete
```  

        component 真实对象的接口

        componentConcrete 真实对象

        decorator 装饰对象基类

        decoratorConcrete 装饰对象实例

        https://www.cnblogs.com/coder-8/p/decorator.html



## 组合模式 ##

    层次体系结构,可表示为部分与整体的关系,树形结构.
    看待对象的角度,整棵树,子树,叶子节点.
    component   整棵树的行为接口
    composite   子树(存储很多子树或叶子节点)
    leaf        叶子节点
    client

    文件夹 公司组织架构都符合这种层次结构
    数据结构中的树 以遍历为例,遍历作为component,composite和leaf实现该接口,对leaf的遍历只需要打印一个节点,对composite的遍历需要打印所有存储的节点.组合模式将多种行为作用于一个对象或者该对象的集合.


## 外观模式 ##

    外观模式将复杂的子系统接口,转化为简单的接口,并不封装子系统.

## 享元模式 ##

    享元模式可以减少内存中对象的数量 使得相同或者相似的对象在内存中只保存一份 从而可以节约系统资源 提高系统性能

    核心是flyweightFactory中的hashmap,存储着多个可共享的对象内部状态,保持每种只存在一个对象,需要新建实例时优先在hashmap中查找,若没有才创建.
    对象的外部状态(不可共享)由客户端通过接口来动态改变,享元抽象类提供这种接口.

    http://gameprogrammingpatterns.com/flyweight.html
    https://blog.csdn.net/knightonhourse/article/details/80003116

## 代理模式 ##

    realSubject和proxySubject都继承subject是为了统一类型,面向接口编程。而不是为了复用。

    代理模式通过代理对象隔离调用方和被调用方,代理对象作为被调用对象的中间层,可以在代理对象内部实现对被调用对象的功能扩展。