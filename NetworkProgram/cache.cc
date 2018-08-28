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

int main(int argc, char *argv[])
{
    TestLFU();
    return 0;
}