
// 迭代器模式测试

#include <iostream>
#include "iset.hpp"
#include "iterator.hpp"
#include "concreteSet.hpp"
#include "concreteIterator.hpp"

int main(int argc, char const *argv[])
{
    // ISet<int> *set = new ConcreteSetVector<int>();
    ISet<int> *set = new ConcreteSetList<int>();
    set->PushItem(3);
    set->PushItem(2);
    set->PushItem(1);

    Iterator<int>* it = set->CreateIterator();

    for (it->First(); !it->IsDone(); it->Next())
    {
        std::cout << *it->CurItem() << std::endl;
    }
    delete it;
    delete set;
    return 0;
}
