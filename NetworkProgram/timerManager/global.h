#ifndef __H_GLOBAL_H__
#define __H_GLOBAL_H__

#include <vector>
#include <string>
#include "log.h"

using namespace std;

#define UNUSED(expr) do { (void)(expr); } while (0)

#define DEF_FRIENDSERVER_REGIONID (0)
#define DEFAULT_GAMEID 0

class TGlobal
{
public:
	static std::string		_ip;
    static short        	_port;  
	static int            	_svid;
    static int            	_level;
	static std::string           _xmlconfig;
	static std::string           _log_path;
    static int              _log_level;
    static int              _log_max_num;
    static int              _log_maxsize;
    static int              _isdaemon;
	static int              _isTestVersion;
	static int				_viewlisttime;

private: //no impl
    TGlobal (void);
    ~TGlobal (void);
    TGlobal (const TGlobal&);
    const TGlobal& operator= (const TGlobal&);
};

#endif

