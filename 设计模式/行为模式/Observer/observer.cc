
//

#include "observer.hpp"
#include "observable.hpp"

Observer::Observer(Observable *s)
{
    sub_ = s;
    sub_->Register(this);
}
Observer::~Observer()
{
    sub_->UnRegister(this);
}