
#include "handlerConcrete.h"

enum
{
    DEBUG,
    INFO,
    ERROR
};

static AbstractLogger* getLoggerChain()
{
    AbstractLogger *remoteLogger = new RemoteLogger(ERROR);
    AbstractLogger *fileLogger = new FileLogger(INFO);
    AbstractLogger *consoleLogger = new ConsoleLogger(DEBUG);

    remoteLogger->setNextLogger(fileLogger);
    fileLogger->setNextLogger(consoleLogger);

    return remoteLogger;
}

int main(int argc, char* argv[])
{
    AbstractLogger *logger =  getLoggerChain();
    logger->logMessage(INFO, "This is an information.");
    logger->logMessage(DEBUG, "This is a debug level information.");
    logger->logMessage(ERROR, "This is an error information.");
    return 0;
}