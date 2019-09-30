
#ifndef _INVOKER_H_
#define _INVOKER_H_

#include "commandConcrete.h"
#include <vector>

// 调用对象
// 封装调用 解除行为请求者和行为实现者的紧耦合关系 通过存储命令对象来实现 
// 命令对象作为中间层解除紧耦合关系 


// 绑定命令对象到调用对象
class RemoteControl
{
public:
    RemoteControl(int n)
    {
        ICommand *noCommand = new NoCommand();
        for (int i = 0; i < n; ++i)
        {
            onCommands_.push_back(noCommand);
            offCommands_.push_back(noCommand);
        }
        undoCommand_ = noCommand;
    }
    void setCommand(int slot, ICommand *onCommand, ICommand *offCommand)
    {
        onCommands_[slot] = onCommand;
        offCommands_[slot] = offCommand;
    }
    void onButtonWasPushed(int slot)
    {
        onCommands_[slot]->execute();
        undoCommand_ = onCommands_[slot];
    }
    void offButtonWasPushed(int slot)
    {
        offCommands_[slot]->execute();
        undoCommand_ = offCommands_[slot];
    }
    void undoButtonWasPushed()
    {
        undoCommand_->undo();
    }
private:
    std::vector<ICommand*> onCommands_;
    std::vector<ICommand*> offCommands_;
    ICommand *undoCommand_;
};

class SimpleRemoteControl 
{
public:
    SimpleRemoteControl()
    {

    }
    void setCommand(ICommand *command)
    {
        slot_ = command;    
    }
    void buttonWasPressed()
    {
        slot_->execute();
    }
private:
    ICommand *slot_;
};
#endif