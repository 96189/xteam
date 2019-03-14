享元模式
    享元模式可以减少内存中对象的数量 使得相同或者相似的对象在内存中只保存一份 从而可以节约系统资源 提高系统性能


核心是flyweightFactory中的hashmap,存储着多个可共享的对象内部状态,保持每种只存在一个对象,需要新建实例时优先在hashmap中查找,若没有才创建.
对象的外部状态(不可共享)由客户端通过接口来动态改变,享元抽象类提供这种接口.

http://gameprogrammingpatterns.com/flyweight.html
https://blog.csdn.net/knightonhourse/article/details/80003116
