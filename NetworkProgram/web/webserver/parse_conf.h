/*************************************************************************
	> File Name: parse_conf.h
	> Author: 
	> Mail: 
	> Created Time: 2017年07月27日 星期四 23时18分34秒
 ************************************************************************/

#ifndef _PARSE_CONF_H
#define _PARSE_CONF_H
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "cJSON.h"

#define CONF_PATH "./config/web.conf"
#define MAXLEN 1024

typedef struct web_conf
{
    int http_port;
    int https_port;
    char *logfile;
    char *category;
}Conf;

Conf my_conf;

int InitConf();
#endif
