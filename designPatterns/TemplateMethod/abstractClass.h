
#ifndef _ABSTRACT_CLASS_H_
#define _ABSTRACT_CLASS_H_

#include <string>
#include <iostream>

class Account 
{
public:
    virtual ~Account() {};
public:
    // 基本方法-具体方法(共享方法)
    bool validData(std::string account, std::string password)
    {
        bool valid = false;
        if (account == "user" && password == "password")
        {
            valid = true;
        }
        return valid;
    }
    void display()
    {
        std::cout << "x:100000000" << std::endl;
    }
    // 基本方法-抽象方法(自定义方法)
    virtual void calculateInterest() = 0;

    // template method
    void Handle(std::string account, std::string password)
    {
        // step 1
        if (!validData(account, password))
        {
            std::cout << "invalid account" << std::endl;
            return;
        }
        // step 2
        calculateInterest();
        // step 3
        display();
    }
};

#endif