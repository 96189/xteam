#include "web.h"

#include <openssl/ssl.h>
#include <openssl/err.h>

const char *web_file_path = "./htdocs";
extern zlog_category_t *c;

#ifdef HTTPS
extern SSL_CTX *g_ctx;
SSL *ssl = NULL;
#endif
/*
 * 获取客户端请求包的内容，打印在日志中
 */
static void read_requesthdrs(rio_t *rp)
{
	assert(rp);
	char buf[MAXLINE] = { 0 };
	Rio_readlineb(rp, buf, MAXLINE);
	//! Host: localhost:8080 
	//zlog_info(c, buf);
	while (strcmp(buf ,"\r\n"))     
	{
		memset(buf, 0, MAXLINE);
		Rio_readlineb(rp, buf, MAXLINE);
		//zlog_info(c, buf);
	}
} 

/*
 * POST方法需要获取post数据的长度
 */
static void post_requesthdrs(rio_t *rp, int *content_length)
{
	assert(rp && content_length);
	char buf[MAXLINE] = { 0 };
	char *p = NULL;

	Rio_readlineb(rp, buf, MAXLINE);
	while (strcmp(buf, "\r\n"))
	{
		Rio_readlineb(rp, buf, MAXLINE);
		if (strncasecmp(buf, "Content-Length:", 15) == 0)
		{
			p = &buf[15];
			p += strspn(p, " \t");
			*content_length = atol(p);
		}
	}
}

/*
 * 获取https包中Content-Length字段的值和从客户端传过来的参数
 * http包中只有Content-Length字段的值，客户端传过来的参数需要下一次read才能读到
 * https包本身是加密的，所以可以将参数同包一起传递
 */
static void post_https_requesthdrs(char *buf, int *content_length, char *cgiargs)
{
	assert(buf && content_length);
	char *p = NULL;
    char line[64] = { 0 };
    char *end = NULL;
	char *ptr = buf;
	while (strcmp(ptr, "\r\n"))
	{
		if ((end = strstr(ptr, "\n")) != NULL)
		{
			if (strncasecmp(ptr,"Content-Length:",15) == 0)
			{
				strncpy(line, ptr, end - ptr);
				p = &line[15];
				p += strspn(p, " \t");
				*content_length = atol(p);
				if (*content_length > 0)
				{
					break;
				}
			}
			ptr = end + 1;
		}
	}
	if ((end = strstr(buf, "\r\n\r\n")) != NULL)
	{
		end = end + strlen("\r\n\r\n");
		strncpy(cgiargs, end, *content_length);
	}
}

static int parse_uri(char *uri, char *filename, char *cgiargs)
{
	assert(uri && filename && cgiargs);

	char *ptr = NULL;
	//! static
	if (!strstr(uri, "cgi-bin"))
	{
		strncpy(cgiargs, "", 1);
		//转换为相对路径
		strncpy(filename, web_file_path, strlen(web_file_path));
		strcat(filename, uri);
		//若请求为目录，则默认文件是./htdocs/home.html
		if (uri[strlen(uri) - 1] == '/')
		{
			strcat(filename, "home.html");
		}
		return 1;
	}
	//! dynamic
	else 
	{
		ptr = index(uri, '?');
		if (ptr)
		{
			strncpy(cgiargs, ptr + 1, strlen(ptr+1));
			*ptr = '\0';
		}
		else 
		{
			strncpy(cgiargs, "", 1);
		}
		//转相对路径
		strncpy(filename, web_file_path, strlen(web_file_path));
		strcat(filename, uri);
		
		return 0;
	}
}

static void get_filetype(char *filename, char *filetype)
{
	assert(filename && filetype);

	if (strstr(filename, ".html"))
	{
		strncpy(filetype, TYPE_HTML, strlen(TYPE_HTML));
	}
	else if (strstr(filename, ".gif"))
	{
		strncpy(filetype, TYPE_GIF, strlen(TYPE_GIF));
	}
	else if (strstr(filetype, ".jpg"))
	{
		strncpy(filetype, TYPE_JPG, strlen(TYPE_JPG));
	}
	else 
	{
		strncpy(filetype, TYPE_PLAIN, strlen(TYPE_PLAIN));
	}
}

static void server_static(int connfd, char *filename, int filesize, const char *method)
{
	assert(filename);
	assert(connfd > 0);
	assert(filesize > 0);

	int srcfd;
	char *srcp;
	char filetype[MAXLINE] = { 0 };
	char buf[MAXBUF] = { 0 };

	get_filetype(filename, filetype);
	sprintf(buf, "HTTP/1.0 200 OK\r\n");
	sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
	sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
	sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
		
	#ifdef HTTPS
		SSL_write(ssl, buf, strlen(buf));
	#else 
		Rio_writen(connfd, buf, strlen(buf));
	#endif

	zlog_info(c, buf);
	if (strcasecmp(method, "GET") == 0)
	{
		srcfd = Open(filename, O_RDONLY, 0);
		srcp = (char *)malloc((filesize + 1) * sizeof(char));
		memset(srcp, 0, filesize + 1);

		Rio_readn(srcfd, srcp, filesize);
		
		zlog_info(c, srcp);
		
		#ifdef HTTPS
			SSL_write(ssl, srcp, filesize);
		#else 
			Rio_writen(connfd, srcp, filesize);
		#endif	
		free(srcp);
		srcp = NULL;
		Close(srcfd);
	}
	else if (strcasecmp(method, "HEAD") == 0)
	{
		//method == "HEAD"
		//只返回服务器信息，不返回实际内容信息
	}
}

void handler_sigchld(int sig)
{
	pid_t pid;
	
	while ((pid = waitpid(-1, NULL, 0)) > 0)
	{
		//log
	}
	if (errno != ECHILD)
	{
		//log
	}
	return;
}

static void handle_sigpipe(int sig)
{
	//zlog_info(c, "..handle_sigpipe...%d", sig);
	return;
}

static void get_dynamic(int connfd, const char *filename, const char *cgiargs)
{
	assert(filename && cgiargs);
	assert(connfd >= 0);

	char buf[MAXLINE] = { 0 };
	char *emptylist[] = { NULL };
	int pipe_in[2];
	int pipe_out[2];
	//处理子进程结束的信号，防止僵尸进程
	Signal(SIGCHLD, handler_sigchld);

	sprintf(buf, "HTTP/1.0 200 OK\r\n");
#ifdef HTTPS
	SSL_write(ssl, buf, strlen(buf));
#else
	Rio_writen(connfd, buf, strlen(buf));	
#endif	
	zlog_info(c, buf);
	sprintf(buf, "Server: Tiny Web Server\r\n");
#ifdef HTTPS
	SSL_write(ssl, buf, strlen(buf));
#else
	Rio_writen(connfd, buf, strlen(buf));
#endif
	zlog_info(c, buf);
	pipe(pipe_in);
	pipe(pipe_out);
	if (Fork() == 0)
	{	
		dup2(pipe_out[1], 1);
		close(pipe_out[0]);
		setenv("QUERY_STRING", cgiargs, 1);
		Execve(filename, emptylist, environ);	
	}
	else 
	{
		//dup2(pipe_out[0],0);
		close(pipe_out[1]);
		char ch;
		while(read(pipe_out[0], &ch, 1) > 0)
		{
		#ifdef HTTPS
			SSL_write(ssl, &ch, 1);
		#else
			send(connfd, &ch, 1, 0);
		#endif
		}
	}
	
}

static void post_dynamic(int connfd, const char *filename, char *post_data, int content_length)
{
	assert(filename && post_data);
	assert(connfd > 0);
	assert(content_length > 0);

	char buf[MAXLINE] = { 0 };
	char *emptylist[] = { NULL };
	int pipe_in[2];
	int pipe_out[2];
	char length_content[32] = { 0 };
	
	sprintf(length_content, "%d", content_length);	
	//zlog_info(c, buf);
	//处理子进程结束的信号，防止僵尸进程
	Signal(SIGCHLD, handler_sigchld);

	pipe(pipe_in);
	pipe(pipe_out);
	if (Fork() == 0)
	{
		dup2(pipe_out[1], 1);
		dup2(pipe_in[0], 0);
		close(pipe_out[0]);
		close(pipe_in[1]);

		setenv("CONTENT-LENGTH", length_content, 1);
		Execve(filename, emptylist, environ);
		exit(0);
	}
	else 
	{
		sprintf(buf, "HTTP/1.0 200 OK\r\n");
	#ifdef HTTPS
		SSL_write(ssl, buf, strlen(buf));
	#else
		Rio_writen(connfd, buf, strlen(buf));	
	#endif
		zlog_info(c, buf);
		sprintf(buf, "Server: Tiny Web Server\r\n");
	#ifdef HTTPS
		SSL_write(ssl, buf, strlen(buf));
	#else
		Rio_writen(connfd, buf, strlen(buf));
	#endif
		zlog_info(c, buf);

		close(pipe_out[1]);
		close(pipe_in[0]);
		//char c;
		//for (int i = 0; i < content_length; i++)
		//{
		//	recv(connfd, &c, 1, 0);
		//	write(pipe_in[1], &c, 1);
		//}
		Rio_writen(pipe_in[1], post_data, content_length);
		while (read(pipe_out[0], &c, 1) > 0)
		{
		#ifdef HTTPS
			SSL_write(ssl,&c,1);
		#else
			send(connfd, &c, 1, 0);
		#endif
		}
		close(pipe_out[0]);
		close(pipe_in[1]);
		
	}
#if 0	
	//! child
	if (Fork() == 0)
	{
		Close(pipe_in[0]);	
                //子进程写post数据到管道中，方便父进程读取
		zlog_info(c, post_data);	
		Rio_writen(pipe_in[1], post_data, content_length);
		zlog_info(c, "child end");
		exit(0);
	}
	//! parent	
	//else 
	//{
		

		sprintf(buf, "HTTP/1.0 200 OK\r\n");
		zlog_info(c, buf);
		Rio_writen(connfd, buf, strlen(buf));	
		sprintf(buf, "Server: Tiny Web Server\r\n");
		zlog_info(c, buf);
		Rio_writen(connfd, buf, strlen(buf));	
		//父进程重定向标准输入到管道读端，后续从标准输入读，相当于在管道中读
		//父进程执行execve 的cgi时，在cgi中read标准输入，相当于从管道中读
		Dup2(pipe_in[0], STDIN_FILENO);
		Close(pipe_in[0]);

		Close(pipe_in[1]);
		setenv("CONTENT-LENGTH", length_content, 1);

		Dup2(connfd, STDOUT_FILENO);
		zlog_info(c, "execve start");
		Execve(filename, emptylist, environ);	
		zlog_info(c, "execve error");		
	//}
#endif
}

#if 0
//! 废弃
static void server_dynamic(int connfd, char *filename, char *cgiargs, int content_length, const char *method)
{
	assert(filename && cgiargs && method);
	assert(connfd > 0);
	assert(content_length >= 0);

	//处理子进程结束的信号，防止僵尸进程
	Signal(SIGCHLD, handler_sigchld);

	//writelog(cgiargs);
	char buf[MAXLINE] = { 0 };
	char *emptylist[] = { NULL };
	char post_data[MAXLINE] = { 0 };
	int pipe_rw[2];
	char length_content[32] = { 0 };

	if (strcmp(method, "GET") == 0)
	{
		sprintf(buf, "HTTP/1.0 200 OK\r\n");
		Rio_writen(connfd, buf, strlen(buf));
		//writelog(buf);
		sprintf(buf, "Server: Tiny Web Server\r\n");
		Rio_writen(connfd, buf, strlen(buf));
		//writelog(buf);
		if (Fork() == 0)
		{
			setenv("QUERY_STRING", cgiargs, 1);
			Dup2(connfd, STDOUT_FILENO);
			//writelog(filename);
			Execve(filename, emptylist, environ);	
		}
	}
	else if (strcmp(method, "POST") == 0)
	{
		writelog("[post temp not impl]\n");
		sprintf(length_content, "%d", content_length);

		pipe(pipe_rw);
		//! child
		if (Fork() == 0)
		{
			Close(pipe_rw[0]);
			
			Rio_writen(pipe_rw[1], post_data, content_length);
			exit(0);
		}
		//! parent	
		else 
		{
			sprintf(buf, "HTTP/1.0 200 OK\r\n");
			Rio_writen(connfd, buf, strlen(buf));	
			sprintf(buf, "Server: Tiny Web Server\r\n");
			Rio_writen(connfd, buf, strlen(buf));	
			
			Dup2(pipe_rw[0], STDIN_FILENO);
			Close(pipe_rw[0]);

			Close(pipe_rw[1]);
			setenv("CONTENT-LENGTH", length_content, 1);

			Dup2(connfd, STDOUT_FILENO);
			Execve(filename, emptylist, environ);			
		}	
	}
	else
	{

	}
}
#endif

static void clienterr(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg)
{

	assert(cause && errnum && shortmsg && longmsg);
	assert(fd > 0);

	char header[MAXLINE] = { 0 };
	char body[MAXBUF] = { 0 };

	sprintf(body, "<html><title>Tiny Error</title>");
	sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
	sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
	sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
	sprintf(body, "%s<hr><em>The Tiny Web Server</em>\r\n", body);
	
	sprintf(header, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
	//zlog_info(c, header);
	//Rio_writen(fd, header, strlen(header));
	sprintf(header, "%s Content-type: text/html\r\n", header);
	//zlog_info(c, header);
	//zlog_info(c, "%d", fd);
	//Rio_writen(fd, header, strlen(header));
	sprintf(header, "%s Content-length: %d\r\n\r\n",header, (int)strlen(body));
	//zlog_info(c, header);
#ifdef HTTPS
	SSL_write(ssl, header, strlen(header));
	SSL_write(ssl, body, strlen(body));
#else
	Rio_writen(fd, header,strlen(header));
	//zlog_info(c, body);
	Rio_writen(fd, body, strlen(body));
#endif
}

const char *support_method[] = {"GET","POST","HEAD"};

static bool is_support_method(const char *method)
{
	assert(method);
	bool is_support = false;
	int i = 0;
	int len = sizeof(support_method) / sizeof(support_method[0]);
	for (i = 0; i < len; i++)
	{
		if (strcmp(support_method[i], method) == 0)
		{
			is_support = true;
		}
	}
	return is_support;
}

void doit(int connfd)
{
	assert(connfd > 0);

	int is_static;
	struct stat sbuf;
	char buf[MAXLINE] = { 0 };
	char method[MAXLINE] = { 0 };
	char uri[MAXLINE] = { 0 };
	char version[MAXLINE] = { 0 };
	char filename[MAXLINE] = { 0 };
	char cgiargs[MAXLINE] = { 0 };
	rio_t rio;
	memset(&rio, 0, sizeof(rio_t));
	int content_length = 0;
	Signal(SIGPIPE, handle_sigpipe);

#ifdef HTTPS
	ssl = SSL_new(g_ctx);
	SSL_set_fd(ssl,connfd);
	if (SSL_accept(ssl) == -1)
	{
		perror("ssl accept");
		return;
	}

	SSL_read(ssl, buf, MAXBUF);
#else 
	Rio_readinitb(&rio, connfd);
	Rio_readlineb(&rio, buf, MAXLINE);
#endif
	//包中读出的内容至少需要有方法，方法名最短的长度是3 GET
	if (strlen(buf) <= 3)
	{
		zlog_info(c, "doit no content");
		goto finish;
	}

	/* GET / HTTP/1.1 */
	zlog_info(c, buf);
	sscanf(buf, "%s %s %s", method, uri, version);
	if (!is_support_method(method))
	{
		clienterr(connfd, method, "501", "Not Implemented", "Tiny does not implement this method.");
		return;
	}

	//read_requesthdrs(&rio);
	

	is_static = parse_uri(uri, filename, cgiargs);
	if (stat(filename, &sbuf) < 0)
	{
		clienterr(connfd, filename, "404", "Not Found", "Tiny could not find this file");
		return;
	}

	if (is_static)
	{
		if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode))
		{
			clienterr(connfd, filename, "403", "Forbidden", "Tiny could not read the file");
			return;
		}
		server_static(connfd, filename, sbuf.st_size, method);
	}
	else 
	{
		if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode))
		{
			clienterr(connfd, filename, "403", "Forbidden", "Tiny could not run the CGI program");
			return;
		}

		//server_dynamic(connfd, filename, cgiargs, content_length, method);
		if (strcmp(method, "GET") == 0)
		{
			get_dynamic(connfd, filename, cgiargs);
		}
		else if (strcmp(method, "POST") == 0)
		{
		#ifdef HTTPS
			post_https_requesthdrs(buf, &content_length, cgiargs);
		#else
			post_requesthdrs(&rio, &content_length);
			//Rio_readlineb函数底层实现i=1，不是从０开始,因此长度需要加１
			Rio_readlineb(&rio, cgiargs, content_length + 1);
		#endif
			post_dynamic(connfd, filename, cgiargs, content_length);
		}
		else 
		{
			zlog_error(c,"fatal error");
		}
		
	}
finish:
#ifdef HTTPS
	SSL_shutdown(ssl);
	SSL_free(ssl);
#endif
	return;
}

