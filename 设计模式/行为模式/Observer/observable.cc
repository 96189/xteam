

#include "observer.hpp"
#include "observable.hpp"
#include <algorithm>

Observable::Observable()
{

}
Observable:: ~Observable()
{

}
//
void Observable::Register(Observer *o)
{
    observers_.push_back(o);
}
void Observable::UnRegister(Observer *o)
{
    std::vector<Observer*>::iterator it = std::find(observers_.begin(), observers_.end(), o);
    if (it != observers_.end())
    {
        std::swap(*it, observers_.back());
        observers_.pop_back();
    }
}
void Observable::Notify()
{
    for (size_t i = 0; i < observers_.size(); ++i)
    {
        Observer *o = observers_[i];
        if (o)
        {
            o->Update();
        }
    }
}