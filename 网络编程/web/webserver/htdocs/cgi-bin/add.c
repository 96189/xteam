 #include "../csapp.h"

int main(int argc, char *argv[])
{
    char *buf = NULL;
    char *p = NULL;
    char number_x[MAXLINE] = { 0 };
    char number_y[MAXLINE] = { 0 };
    char content[MAXLINE] = { 0 };
    int content_length = 0;
    int x = 0;
    int y = 0;
    char *x_pos = NULL;
    char *y_pos = NULL;

    //! GET
    if ((buf = getenv("QUERY_STRING")) != NULL)
    {
	strncpy(content, buf, strlen(buf));
    }
    //! POST
    else if ((buf = getenv("CONTENT-LENGTH")) != NULL)
    {
        content_length = atol(buf);
	read(STDIN_FILENO, content, content_length);
    }
    else 
    {
        //log
    }

    if (strlen(content) > 0)
    {
        p = strstr(content, "&");
        *p = '\0';
        strncpy(number_x, content, strlen(content));
        strncpy(number_y, p + 1, strlen(p + 1));
        x_pos = strstr(number_x, "=");
        y_pos = strstr(number_y, "=");
        x = atoi(x_pos + 1);
        y = atoi(y_pos + 1);   
    }
    
    memset(content, 0, MAXLINE);
    sprintf(content, "welcome to add.com: ");
    sprintf(content, "%s the internet addition portal.\r\n<p>", content);
    sprintf(content, "%s the answer is: %d + %d = %d\r\n<p>", content, x, y, x + y);
    sprintf(content, "%s thanks for visiting!\r\n", content);

    printf("Content-length: %d\r\n", (int)strlen(content));
    printf("Content-type: text/html\r\n\r\n");
    printf("%s", content);
    fflush(stdout);
    //exit(0);
}
