#include "global.h"


#ifndef __H_LOGIC_H__
#define __H_LOGIC_H__

#define TIME_INTR_VAL (5*60)

#define STATIC_BUFF_LEN (512)


class CLogic
{
    public:
        CLogic(){;}
        ~CLogic(){;}

        static string TimeStamp2String(time_t tt)
        {
            struct tm * timeinfo;
            timeinfo = localtime ((time_t*)&tt);
            return asctime(timeinfo);
        }


        static string GetDateString(time_t tstamp)
        {
            struct tm* timeinfo;
            timeinfo=localtime(&tstamp);
			char g_strbuff[512] = {0};
            strftime(g_strbuff,STATIC_BUFF_LEN,"%Y%m%d",timeinfo);

            return g_strbuff;
        }

        static string GetWeekString(time_t tstamp)
        {
            struct tm* timeinfo;
            timeinfo=localtime(&tstamp);
			char g_strbuff[512] = { 0 };
            strftime(g_strbuff,STATIC_BUFF_LEN,"%Y%m",timeinfo);

            return g_strbuff;
        }
	
        static int GetDateLeastTime(time_t tstamp)
        {
            struct tm timeinfo;
            localtime_r(&tstamp, &timeinfo);
            int now_time_second = timeinfo.tm_hour*3600 + timeinfo.tm_min*60 + timeinfo.tm_sec;
            int total_time_second = 24 * 3600 + 1;

            int intrval=total_time_second-now_time_second;
            if(intrval<=0)
            {
                intrval=0;
            }

            return intrval;
        }

		static int GetCurrDayLeastTime()
		{
			time_t current = time(NULL);
			return GetDateLeastTime(current);
		}


        static int GetWeekLeastTime(time_t tstamp)
        {
            struct tm timeinfo;
            localtime_r(&tstamp, &timeinfo);
            int wday=(timeinfo.tm_wday==0)?7:timeinfo.tm_wday;
            int now_time_second = (wday-1)*24*3600 + timeinfo.tm_hour*3600 + timeinfo.tm_min*60 + timeinfo.tm_sec;
            int total_time_second = 7*24 * 3600 + 1;

            int intrval=total_time_second-now_time_second;
            if(intrval<=0)
            {
                intrval=0;
            }

            return intrval;
        }


};


#endif


