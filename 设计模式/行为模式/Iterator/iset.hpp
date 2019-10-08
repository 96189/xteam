
// 集合接口类型

#ifndef _ISET_HPP_
#define _ISET_HPP_

#include "iterator.hpp"

template <typename Item>
class ISet 
{
public:
    ISet() {}
    virtual ~ISet() {}

public:
    virtual void PushItem(Item item) = 0;
    virtual Item& operator[](int location) = 0;
    virtual Iterator<Item>* CreateIterator() = 0;
    virtual int GetSize() = 0;
};

#endif
