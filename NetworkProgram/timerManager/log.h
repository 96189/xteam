#ifndef __TTCLOG_H__
#define __TTCLOG_H__

#include <sys/cdefs.h>

__BEGIN_DECLS

#include <stdio.h>

#define DEFAULT_FILE_NUM    10
#define DEFAULT_FILE_SIZE   1*1024*1024*1024

#if CLIENTAPI
	#define __log_level__ 6
#else
	extern int __log_level__;
	extern bool __log_stdout__;
#endif

enum LOG_LEVEL
{
	log_level_boot = -1,
	log_level_emerg = 0,
	log_level_alert = 1,
	log_level_crit = 2,
	log_level_error = 3,
	log_level_warning = 4,
	log_level_notice = 5,
	log_level_info = 6,
	log_level_debug = 7,
};

#define log_generic(lvl, fmt, args...)	 write_log(lvl, __FILE__, __FUNCTION__, __LINE__ , fmt, ##args)
#define log_access(accesslog, buf, fmt, args...) write_access(accesslog, buf , fmt, ##args)
#define log_boot(fmt, args...)		log_generic(log_level_boot, fmt, ##args)
#define log_emerg(fmt, args...)	    log_generic(log_level_emerg, fmt, ##args)
#define log_alert(fmt, args...)		log_generic(log_level_alert, fmt, ##args)
#define log_crit(fmt, args...)		log_generic(log_level_crit, fmt, ##args)
#define log_error(fmt, args...)		log_generic(log_level_error, fmt, ##args)
#define log_warning(fmt, args...)	do{ if(__log_level__>=log_level_warning)log_generic(4, fmt, ##args); } while(0)
#define log_notice(fmt, args...)	do{ if(__log_level__>=log_level_notice)log_generic(5, fmt, ##args); } while(0)
#define log_info(fmt, args...)		do{ if(__log_level__>=log_level_info)log_generic(6, fmt, ##args); } while(0)
#define log_debug(fmt, args...)		do{ if(__log_level__>=log_level_debug)log_generic(7, fmt, ##args); } while(0)

#define error_log(fmt, args...)		log_error(fmt, ##args)

#if CLIENTAPI
	#if __cplusplus
		static inline void init_log (const char *app, const char *dir = NULL, int max_num = -1, int max_size = -1) {}
	#else
		static inline void init_log (const char *app, const char *dir, int max_num, int max_size) {}
	#endif
	static inline void set_log_level(int n){}
	static inline void write_log (int n, const char *a, const char *b, int c, const char *d, ...) {}
#else
	#if __cplusplus
		extern void init_log (const char *app, const char *dir = NULL, int max_num = -1, int max_size = -1);
	#else
		extern void init_log (const char *app, const char *dir, int max_num, int max_size);
	#endif
	extern void set_log_level(int);
	extern void set_log_stdout(bool);
	extern void write_log (int, const char*, const char *, int, const char *, ...) __attribute__((format(__printf__,5,6)));
	extern void write_access(int access, const char* rsp_buf, const char* fmt, ...) __attribute__((format(__printf__,3,4)));
	extern void write_data_log(const char* str_data, int data_len);
#endif

__END_DECLS

#endif
