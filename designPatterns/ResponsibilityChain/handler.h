
#ifndef _HANDLER_H_
#define _HANDLER_H_

#include <string>

class AbstractLogger
{
public:
    AbstractLogger(int level)
    : level_(level), loggerNext_(NULL)
    {
        
    }
    virtual ~AbstractLogger(){}

    void setNextLogger(AbstractLogger *next)
    {
        loggerNext_ = next;
    }
    void logMessage(int level, std::string message)
    {
        if (level_ <= level)
        {
            writeMessage(message);
        }
        if (loggerNext_)
        {
            loggerNext_->logMessage(level, message);
        }
    }

public:
    virtual void writeMessage(std::string message) = 0;

protected:
    int level_;
    AbstractLogger *loggerNext_;
};

#endif