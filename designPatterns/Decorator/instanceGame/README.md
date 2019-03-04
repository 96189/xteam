
                component   <--->   client       // componentConcrete和decorator实现相同的接口是为了client可以不加区分的使用真实对象和装饰对象的接口
                |       |                        // 继承是为了统一类型 装饰对象可以取代真实对象
componentConcrete       decorator
                            |
                            decoratorConcrete

component 真实对象的接口

componentConcrete 真实对象

decorator 装饰对象基类

decoratorConcrete 装饰对象实例

https://www.cnblogs.com/coder-8/p/decorator.html

为真实对象扩展功能(不使用继承),通过decorator持有component的实例对象componentConcrete的引用,在decoratorConcrete中组合行为,
decoratorConcrete构造函数的参数是component引用,decoratorConcrete对象内部保存旧的行为,然后在类方法中实现新的行为并与旧的行为组合。