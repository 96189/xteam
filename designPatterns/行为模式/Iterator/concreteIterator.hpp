
// 具体的迭代器

#ifndef _CONCRETE_ITERATOR_HPP_
#define _CONCRETE_ITERATOR_HPP_

#include "iterator.hpp"
#include "iset.hpp"

template <typename Item>
class ConcreteIterator : public Iterator<Item>
{
private:
    ISet<Item> *set_;   // 集合接口类型
    int cursor_;        // 操作游标

public:
    // 构造 析构
    ConcreteIterator(ISet<Item> *set)
        : set_(set), cursor_(-1)
    {

    }
    virtual ~ConcreteIterator()
    {

    }
    // 接口实现
    virtual void First()
    {
        cursor_ = 0;
    }
    virtual void Next()
    {
        ++cursor_;
    }
    virtual bool IsDone()
    {
        return cursor_ >= set_->GetSize();
    }
    virtual Item* CurItem()
    {
        if (IsDone()) return NULL;
        return &(*set_)[cursor_];
    }
};

#endif