#include <iostream>
#include "timerManager.h"
#include <unistd.h>
#include "globalExtern.h"
#include "global.h"
#include "Logic.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>
////////////////////////////////////////////
#define DEFAULT_LOG_PATH "./log"
#define DEFAULT_LOG_LEVEL log_level_debug

std::string  TGlobal::_ip = "0.0.0.0";
std::string  TGlobal::_log_path = DEFAULT_LOG_PATH;
int     TGlobal::_log_level = DEFAULT_LOG_LEVEL;
int     TGlobal::_log_max_num = DEFAULT_FILE_NUM;
int     TGlobal::_log_maxsize = DEFAULT_FILE_SIZE;
short   TGlobal::_port = 0;
int     TGlobal::_svid = 0;
int     TGlobal::_level = 0;
int     TGlobal::_isdaemon = 0;
int     TGlobal::_isTestVersion = 0;
int 	TGlobal::_viewlisttime = 5;
std::string  TGlobal::_xmlconfig = "config.xml";

std::string GetLogFileName()
{
	return "TimerManager";
}

void InitDaemonLog()
{
	char logfile[256];
	char logpath[256];
	time_t curtime = time(NULL);
	snprintf(logpath, sizeof(logpath), "%s/%s/", TGlobal::_log_path.c_str(), CLogic::GetDateString(curtime).c_str());
	snprintf(logfile, sizeof(logfile), "%s.%d.%d.%d.log", GetLogFileName().c_str(), TGlobal::_svid, TGlobal::_level, getpid());
	set_log_level(TGlobal::_log_level);
	set_log_stdout(false);
	init_log(logfile, logpath, TGlobal::_log_max_num, TGlobal::_log_maxsize);

	return;
}

void DaemonStart(void)
{
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGHUP, &sa, NULL);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	sigset_t sset;
	sigemptyset(&sset);
	sigaddset(&sset, SIGSEGV);
	sigaddset(&sset, SIGBUS);
	sigaddset(&sset, SIGABRT);
	sigaddset(&sset, SIGILL);
	sigaddset(&sset, SIGCHLD);
	sigaddset(&sset, SIGFPE);
	sigprocmask(SIG_UNBLOCK, &sset, &sset);
	daemon(1, 1);

	/* Redirect standard files to /dev/null */
	freopen("/dev/null", "r", stdin);
	freopen("/dev/null", "w", stdout);
	freopen("/dev/null", "w", stderr);
}

int s_argc = 0;
char **s_argv = NULL;
int GetArgc() { return s_argc; }
char **GetArgv() { return s_argv; }

int ParseArgs(int argc, char ** argv)
{
	s_argc = argc;
	s_argv = argv;
	char* option = "h:p:s:l:i:d::t";
	int result = 0, errflg = 0;
	extern int  optopt;
	while ((result = getopt(argc, argv, option)) != -1)
	{
		switch (result)
		{
		case 'h':
			TGlobal::_ip = optarg;
			break;
		case 'p':
			TGlobal::_port = atoi(optarg);
			break;
		case 's':
			TGlobal::_svid = atoi(optarg);
			break;
		case 'l':
			TGlobal::_level = atoi(optarg);
			break;
		case 'd':
			DaemonStart();
			TGlobal::_isdaemon = 1;
			break;
		case 't':
			TGlobal::_isTestVersion = 1;
			break;
		case 'i':
			break;
		case ':':
			fprintf(stderr, "Option -%c requires an operand\n", optopt);
			errflg++;
			break;
		case '?':
			fprintf(stderr, "Unrecognized option: -%c\n", optopt);
			errflg++;
			break;
		default:
			errflg++;
			break;
		}
	}

	if (errflg /*|| argc < 2*/)
	{
		fprintf(stderr, "Read argv cmd error,  exp: ./bin -h 127.0.0.1  -p 80 -d \n");
		exit(0);
	}

	if (TGlobal::_isdaemon)
	{
		//TGlobal::_log_path="./log";
		TGlobal::_log_level = log_level_debug;
		InitDaemonLog();
	}

	return 0;
}
////////////////////////////////////////////
void ProcTimerOut(int timerId)
{
    std::cout << timerId << std::endl;
}

int main(int argc, char* argv[])
{
	set_log_level(TGlobal::_log_level);
	set_log_stdout(true);
	ParseArgs(argc, argv);

    TimerManager tm;
    tm.StartTimer(1,2);
    tm.SetTimerOutCallback(ProcTimerOut);
    tm.CheckExpired(3);
    tm.StopTimer(1);
    log_boot("end");
    return 0;
}