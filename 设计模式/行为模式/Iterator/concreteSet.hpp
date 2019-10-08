
// 具体的集合类型

#ifndef _CONCRETE_SET_HPP_
#define _CONCRETE_SET_HPP_

#include "iset.hpp"
#include "concreteIterator.hpp"
#include <vector>
#include <list>

template <typename Item>
class ConcreteSetVector : public ISet<Item>
{
private:
    std::vector<Item> dataSet_;

public:
    // 构造 析构
    ConcreteSetVector()
    {
        dataSet_.clear();
    }
    ~ConcreteSetVector()
    {
        dataSet_.clear();
    }
    // 接口实现
    virtual void PushItem(Item item) 
    {
        dataSet_.push_back(item);
    }
    virtual Item& operator[](int location) 
    {
        return dataSet_[location];
    }
    virtual Iterator<Item>* CreateIterator()
    {
        return new ConcreteIterator<Item>(this);
    }
    virtual int GetSize()
    {
        return dataSet_.size();
    }
};


template <typename Item>
class ConcreteSetList : public ISet<Item>
{
private:
    std::list<Item> dataSet_;

public:
    // 构造 析构
    ConcreteSetList()
    {
        dataSet_.clear();
    }
    ~ConcreteSetList()
    {
        dataSet_.clear();
    }
    // 接口实现
    virtual void PushItem(Item item) 
    {
        dataSet_.push_back(item);
    }
    virtual Item& operator[](int location) 
    {
        typename std::list<Item>::iterator it = dataSet_.begin();
        for ( ; location-- && it != dataSet_.end(); ++it);
        // if (!location || it == dataSet_.end()) return Item();
        return (*it);
    }
    virtual Iterator<Item>* CreateIterator()
    {
        return new ConcreteIterator<Item>(this);
    }
    virtual int GetSize()
    {
        return dataSet_.size();
    }
};

#endif