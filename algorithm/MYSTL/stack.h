#include "vector.h"
// #include "list.h"

// using namespace MYSTL;
namespace MYSTL 
{

// 基于Vector的stack
template <typename T>
class Stack : public Vector<T>
{
public:
    // default constructor
public:
    void push(const T& e)
    {
        insert(this->size(), e);
    }
    T pop()
    {
        return this->remove(this->size() - 1);
    }
    T& top()
    {
        return (*this)[this->size() - 1];
    }
};

// 基于List的stack

};