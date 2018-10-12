
https://www.cnblogs.com/Philip-Tell-Truth/p/6284475.html
## 管道
    int pipe(int fd[2])
    匿名管道 父子进程间使用
    半双工 一端读一端写 数据流向是单向的 具体的方向取决于pipe执行后 fork完成 父子进程对管道的处理方式

    int mkfifo(const char *path, mode_t mode)
    命名管道 不同进程也能交换数据

## System V IPC
    标识符 内部命名  每个内核IPC结构都用一个非负整数的标识符加以引用
    键     外部命名  IPC创建函数的参数key_t 该键由内核将其变换为标识符
### 消息队列
    // 通过传入外部键 由内核返回内部标识符
    int msgget(key_t key, int flag)
    
    // 通过标识符操作
    int msgctl(int msgid, int cmd, struct msqid_ds *buf)
    int msgsnd(int msgid, const void *ptr, size_t nbytes, int flag)
    int msgrcv(int msgid, void *ptr, size_t nbytes, long type, int flag)

### 信号量
    // 通过传入外部键 由内核返回内部标识符
    int semget(key_t key, int nsems, int flag)

    // 通过标识符操作
    int semctl(int semid, int semnum, int cmd, ...)
    int semop(int semid, struct sembuf semoparray[], size_t nops)

### 内存映射
    // 通过传入外部键 由内核返回内部标识符
    int shmget(key_t key, size_t size, int flag)

    // 通过标识符操作
    int shmctl(int shmid, int cmd, struct shmid_ds *buf)
    void *shmat(int shmid, const void *addr, int flag)
    int shmdt(const void *addr)

### 缺点
    声明周期随系统 ipc结构在系统范围内起作用 没有引用计数
    ipc结构在文件系统中没有名字,不适用文件描述符无法进行某些操作



## unix域套接字
    int socketpair(int domain, int type, int protocol, int sockfd[])
    一对相互连接的unix域套接字可以起到全双工管道的作用 两端读和写开放
