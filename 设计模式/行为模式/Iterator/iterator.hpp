
// 迭代器接口类型

#ifndef _ITERATOR_HPP_
#define _ITERATOR_HPP_

template <typename Item>
class Iterator 
{
public:
    Iterator() {}
    virtual ~Iterator() {}

public:
    virtual void First() = 0;
    virtual void Next() = 0;
    virtual bool IsDone() = 0;

    virtual Item* CurItem() = 0;
};



#endif