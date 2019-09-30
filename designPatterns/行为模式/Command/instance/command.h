
#ifndef _COMMAND_H_
#define _COMMAND_H_

class ICommand
{
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
};

#endif