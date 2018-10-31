#include "parse_conf.h"

int InitConf()
{
    char buf[MAXLEN] = { 0 };

    int fd = open(CONF_PATH, O_RDONLY);
    if (fd < 0)
    {
        //
        //printf("fd < 0\n");
    }
    read(fd, buf, MAXLEN);
    close(fd);
    cJSON *root = cJSON_Parse(buf);
    my_conf.http_port = cJSON_GetObjectItem(root, "http_port")->valueint;
    my_conf.https_port = cJSON_GetObjectItem(root, "https_port")->valueint;
    my_conf.logfile = cJSON_GetObjectItem(root, "logfile")->valuestring;
    my_conf.category = cJSON_GetObjectItem(root, "category")->valuestring;
    //printf("port=%d  logfile=%s\n",my_conf.port, my_conf.logfile);   
}

//int main()
//{
//    InitConf();
//}
