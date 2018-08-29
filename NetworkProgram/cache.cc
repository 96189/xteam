#include "cache.h"
#include <string>

void TestLFU()
{
    // LFUCache<int,int> lfu;
    // int v;
    // lfu.Set(1,11);
    // lfu.Set(2,22);
    // lfu.Set(3,33);
    // lfu.Set(1,111);
    // lfu.Get(1,v);
    // lfu.Get(1,v);
    // lfu.Get(1,v);
    // lfu.Get(2,v);
    // lfu.Get(2,v);
    // lfu.Get(3,v);
    // lfu.Get(3,v);
    // lfu.Set(4,44);
    // lfu.Get(4,v);
    // // printf("%d\n", v);
    // lfu.PrintTable();
    // printf("%d\n",lfu.Used());

    LFUCache<std::string,std::string> lfu;
    std::string v;
    lfu.Set("qwe", "123");
    lfu.Set("asd", "123");
    lfu.Set("zxc", "123");
    lfu.Get("qwe", v);
    lfu.Get("qwe", v);
    lfu.Get("qwe", v);
    lfu.Get("qwe", v);
    lfu.Get("qwe", v);

    lfu.Get("asd", v);
    lfu.Get("zxc", v);
    lfu.Get("zxc", v);
    lfu.Set("qaz", "111");
    lfu.Get("qaz", v);
    std::cout << v << std::endl;
    lfu.PrintTable();
}

void TestLRU()
{
    // LRUCache<int,int> lru;
    // int v;
    // lru.Set(1,11);
    // lru.Set(2,22);
    // lru.Set(3,33);
    // lru.Get(1,v);
    // lru.Get(1,v);
    // lru.Get(1,v);
    // lru.Get(1,v);
    // lru.Get(2,v);
    // lru.Get(3,v);
    // lru.Set(4,44);
    // lru.Set(5,55);
    // lru.PrintTable();

    LRUCache<std::string,std::string> lru;
    std::string v;
    lru.Set("qwe", "123");
    lru.Set("asd", "123");
    lru.Set("zxc", "123");
    lru.Get("qwe", v);
    lru.Get("qwe", v);
    lru.Get("qwe", v);
    lru.Get("qwe", v);
    lru.Get("qwe", v);

    lru.Get("asd", v);
    lru.Get("zxc", v);
    lru.Get("zxc", v);
    lru.Set("qaz", "111");
    lru.Get("qaz", v);
    std::cout << v << std::endl;
    lru.PrintTable();
}

int main(int argc, char *argv[])
{
    TestLFU();
    TestLRU();
    return 0;
}