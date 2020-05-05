#include "web.h"
#include "parse_conf.h"
#include <string.h>


//#define DIGIT_CERT "server.crt"
//#define USER_PRIVATE_KEY "server.key"

extern Conf my_conf;

int rc = 0;
zlog_category_t *c = NULL;

#ifdef HTTPS
#include <openssl/ssl.h>
#include <openssl/err.h>

#define DIGIT_CERT "./security/cacert.pem"
#define USER_PRIVATE_KEY "./security/privkey.pem"
//ssl
SSL_CTX *g_ctx = NULL;
SSL *g_ssl = NULL;


static void Init_Https()
{
    /* SSL 库初始化*/
    SSL_library_init();
    /* 载入所有SSL 算法*/
    OpenSSL_add_all_algorithms();
    /* 载入所有SSL 错误消息*/
    SSL_load_error_strings();
    /* 以SSL V2 和V3 标准兼容方式产生一个SSL_CTX ，即SSL Content Text */
    g_ctx = SSL_CTX_new(SSLv23_server_method());
    /* 
    也可以用SSLv2_server_method() 或SSLv3_server_method() 单独表示V2 或V3标准
    */
    if (g_ctx == NULL) 
    {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    /* 载入用户的数字证书， 此证书用来发送给客户端。证书里包含有公钥*/
    if (SSL_CTX_use_certificate_file(g_ctx, DIGIT_CERT, SSL_FILETYPE_PEM) <= 0) 
    {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    /* 载入用户私钥*/
    if (SSL_CTX_use_PrivateKey_file(g_ctx, USER_PRIVATE_KEY, SSL_FILETYPE_PEM) <= 0) 
    {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
    /* 检查用户私钥是否正确*/
    if (!SSL_CTX_check_private_key(g_ctx)) 
    {
        ERR_print_errors_fp(stdout);
        exit(1);
    }
}
#endif 

// test for gprof
static void handler_signal(int sig_no) {
    if (SIGUSR1 & sig_no) {
        exit(0);
    }
}

int main(int argc, char *argv[])
{
	int listenfd;
	int connfd;
	int clientlen;
	struct sockaddr_in clientaddr;
        	
        InitConf();
#ifdef HTTPS	
        Init_Https();
#endif
	rc = zlog_init(my_conf.logfile);
	if (rc)
	{
		printf("zlog_init failed\n");
		return -1;
	}

	c = zlog_get_category(my_conf.category);
	if (!c)
	{
		printf("get cat fail\n");
		zlog_fini();
		return -2;
	}

    // test for gprof
    signal(SIGUSR1, handler_signal);
#ifdef HTTPS
    listenfd = Open_listenfd(my_conf.https_port);
#else	
	listenfd = Open_listenfd(my_conf.http_port);
#endif
	
	while (1)
	{
		clientlen = sizeof(clientaddr);
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

		if (connfd > 0)		
		{
			zlog_info(c, "connfd=%d", connfd);
			doit(connfd);
		}

		Close(connfd);
		connfd = -1;
	}
	close(listenfd);
	
#ifdef HTTPS
	SSL_CTX_free(g_ctx);
#endif
	zlog_fini();
}
