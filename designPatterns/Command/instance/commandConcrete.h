
#ifndef _COMMAND_CONCRETE_H_
#define _COMMAND_CONCRETE_H_

#include "command.h"
#include "receiverConcrete.h"

// 命令对象
// 封装命令

// 绑定执行对象到命令对象
class LightOnCommand : public ICommand 
{
public:
    LightOnCommand(Light* light)
    : light_(light)
    {

    }
    virtual void execute()
    {
        light_->on();
    }
    virtual void undo()
    {
        light_->off();
    }
private:
    Light* light_;
};
class LightOffCommand : public ICommand
{
public:
    LightOffCommand(Light* light)
    : light_(light)
    {

    }
    virtual void execute()
    {
        light_->off();
    }
    virtual void undo()
    {
        light_->on();
    }
private:
    Light* light_;
};

class AirconOnCommand : public ICommand
{
public:
    AirconOnCommand(Aircon* aricon)
    : aricon_(aricon)
    {

    }
    virtual void execute()
    {
        aricon_->on();
        aricon_->setMode(0);
        aricon_->setTemperature(24);
    }
    virtual void undo()
    {
        aricon_->off();
    }
private:
    Aircon* aricon_;
};

class AirconOffCommand : public ICommand
{
public:
    AirconOffCommand(Aircon* aricon)
    : aricon_(aricon)
    {

    }
    virtual void execute()
    {
        aricon_->off();
    }
    virtual void undo()
    {
        aricon_->on();
        aricon_->setMode(0);
        aricon_->setTemperature(24);
    }
private:
    Aircon* aricon_;
};

class StereoOnCommand : public ICommand
{
public:
    StereoOnCommand(Stereo *stereo)
    : stereo_(stereo)
    {

    }
    virtual void execute()
    {
        stereo_->on();
        stereo_->setCD();
        stereo_->setVolume(11);
    }
    virtual void undo()
    {
        stereo_->off();
    }
private:
    Stereo *stereo_;
};
class StereoOffCommand : public ICommand
{
public:
    StereoOffCommand(Stereo *stereo)
    : stereo_(stereo)
    {

    }
    virtual void execute()
    {
        stereo_->off();
    }
    virtual void undo()
    {
        stereo_->on();
        stereo_->setCD();
        stereo_->setVolume(11);        
    }
private:
    Stereo *stereo_;
};

class TVOnCommand : public ICommand
{
public:
    TVOnCommand(TV *tv)
    : tv_(tv)
    {

    }
    virtual void execute()
    {
        tv_->on();
    }
    virtual void undo()
    {
        tv_->off();
    }
private:
    TV *tv_;
};
class TVOffCommand : public ICommand
{
public:
    TVOffCommand(TV *tv)
    : tv_(tv)
    {

    }
    virtual void execute()
    {
        tv_->off();
    }
    virtual void undo()
    {
        tv_->on();
    }
private:
    TV *tv_;
};

class NoCommand : public ICommand
{
public:
    NoCommand()
    {

    }
    virtual void execute()
    {

    }
    virtual void undo()
    {

    }
};
#endif