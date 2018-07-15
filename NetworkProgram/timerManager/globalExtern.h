#ifndef __H_GLOBAL_EXTERN_H__
#define __H_GLOBAL_EXTERN_H__

#include <vector>
#include <string>
// #include "Event.h"
// class ServerBase;

//--------------------------------------
//CCReacor export global function  & attribute
//--------------------------------------
// extern ServerBase* GetGameInstance();
extern void InitDaemonLog();
extern bool IsTestVersion();
//extern int SendCCEvent(CCEvent&  event);
extern int GetArgc();
extern char **GetArgv();


//--------------------------------------
//CCReacor import global function & attribute
//--------------------------------------
extern std::string GetLogFileName();
// extern ServerBase* CreateGame();

#endif //__H_GLOBAL_EXTERN_H__