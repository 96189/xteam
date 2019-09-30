#include "list.h"

namespace MYSTL
{

template <typename T>
class Queue : public List<T>
{
public:
    // default constructor
public:
    void enqueue(const T& e)
    {
        List<T>::insertAsLast(e);
    }
    T dequeue()
    {
        T val = this->remove(this->first());
        return val;
    }
    T& front()
    {
        return this->first()->getData();
    }
    T& rear()
    {
        return this->last()->getData();
    }
}; // Queue

}; // MYSTL