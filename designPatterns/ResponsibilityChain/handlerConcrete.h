
#ifndef _HANDLER_CONCRETE_H_
#define _HANDLER_CONCRETE_H_

#include "handler.h"
#include <iostream>

class ConsoleLogger : public AbstractLogger
{
public:
    ConsoleLogger(int level)
    : AbstractLogger(level)
    {

    }
protected:
    virtual void writeMessage(std::string message)
    {
        std::cout << "ConsoleLogger::" << level_ << ":" << message << std::endl;
    }
};

class RemoteLogger : public AbstractLogger
{
public:
    RemoteLogger(int level)
    : AbstractLogger(level)
    {

    }
protected:
    virtual void writeMessage(std::string message)
    {
        std::cout << "RemoteLogger::" << level_ << ":" << message << std::endl;
    }
};

class FileLogger : public AbstractLogger
{
public:
    FileLogger(int level)
    : AbstractLogger(level)
    {

    }

protected:
    virtual void writeMessage(std::string message)
    {
        std::cout << "FileLogger::" << level_ << ":" << message << std::endl;
    }
};

#endif