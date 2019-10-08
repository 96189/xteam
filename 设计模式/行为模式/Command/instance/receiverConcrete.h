
#ifndef _RECEIVER_CONCRETE_H_
#define _RECEIVER_CONCRETE_H_

#include "receiver.h"
#include <string>
#include <iostream>

// 执行对象
// 封装执行命令的对象
// 灯
class Light : public IReceiver 
{
public:
    Light(std::string name)
    : name_(name)
    {

    }
public:
    void on()
    {
        std::cout << name_ << " Light on" << std::endl;
    }
    void off()
    {
        std::cout << name_ << " Light off" << std::endl;
    }
private:
    std::string name_;
};

// 空调
class Aircon : public IReceiver
{
public:
    Aircon(std::string name)
    : name_(name)
    {

    }
public:
    void on()
    {
        std::cout << name_ << " Aircon on" << std::endl;
    }
    void off()
    {
        std::cout << name_ << " Aircon off" << std::endl;
    }
    void setMode(int mode)
    {
        std::cout << name_ << " Aircon mode " << mode << std::endl;
    }
    void setTemperature(int val)
    {
        std::cout << name_ << " Aircon temperature " << val << std::endl;
    }
private:
    std::string name_;
};

// 音响
class Stereo : public IReceiver
{
public:
    Stereo()
    {

    }
public:
    void on()
    {
        std::cout << " Stereo on " << std::endl;
    }
    void off()
    {
        std::cout << " Stereo off " << std::endl;
    }
    void setCD()
    {
        std::cout << " Stereo cd " << std::endl;
    }
    void setVolume(int val)
    {
        std::cout << " Stereo volume " << val << std::endl;
    }
};

// 电视
class TV : public IReceiver
{
public:
    TV()
    {

    }
public:
    void on()
    {
        std::cout << " TV on " << std::endl;
    }
    void off()
    {
        std::cout << " TV off " << std::endl;
    }
};

#endif