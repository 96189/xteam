
## 信号机制
    软件中断 
    异步事件处理方法

### 系统本身支持的信号 
    linux 31种系统信号
### 应用程序自定义的信号
    nginx自定义信号

## 产生信号的条件
### 硬件条件
    SIGFPE 除0
    SIGSEGV 无效内存引用
### 软件条件 
    SIGURG 网络连接上的带外数据) 
    SIGPIPE 网读端关闭的管道上些数据) 
    SIGALRM 进程alarm函数设置的定时器超时)
### 用户主动操作触发信号
    进程调用kill(2)函数
    用户执行kill(1)命令
    用户按某些终端键

## 信号处理的3种方式
### 忽略信号 SIG_IGN
### 捕捉信号 
    执行自定义的信号处理函数
    函数原型 void (*signal(int signo, void (*func)(int)))(int)
    4.4BSD实现signal函数 底层实质是调用sigaction函数

### 执行系统默认动作 SIG_DEL
    SIGKILL SIGSTOP信号只能执行系统默认动作 不能忽略 不能捕捉 大多数信号默认动作是 终止+core

## 不可靠信号到可靠信号的演变
### 早期信号不可靠的两个问题
    问题1、进程在收到信号对其进行处理时 将信号动作重置为默认值 因此信号可能会丢失 某信号已经发生 但进程可能一直不知道
    问题2、进程不希望某种信号发生 它不能关闭该信号 只能忽略 进程希望通知系统 阻止某些信号发生 如果确实已经产生记住它们

### 不可靠信号示例代码
    int sig_int_flag;
    int sig_int()
    {
        ... 
        ~~~ 在这里发生信号
        signal(SIGINT, sig_int);
        sig_int_flag = 1;
    }
    int main(int argc, char *argv[])
    {
        signale(SIGINT, sig_int);
        ...
        while (sig_int_flag == 0)
        {
            ~~~ 在这里发生信号
            pause();
        }
    }
### 不可靠信号分析
    信号发生后到信号处理函数调用signal重新关联信号与处理函数之前的时间段内 发生另一次中断信号 第二个信号执行默认动作 终止进程,造成问题1
    在测试sig_int_flag之后调用pause之前发生信号 若此信号之后不会再产生 则进程永久休眠 

    根本原因在于异步事件的随机性

### 如何使应用程序不必处理被中断的系统调用
#### 术语:
    低速系统调用:可能会使进程永远阻塞的一类系统调用

#### 常见低速系统调用
    recv read 
    send write 
    accept connect
    pause wait
    ioctl
    某些进程间通信函数

#### 早期unix系统特性
    如果进程执行某低速系统调用 在阻塞期间捕捉到信号 系统调用会被信号中断不再继续执行 系统调用返回出错errno设置为EINTR 进程应该以以下示例唤醒被中断的系统调用
    again:
        if ((n = read(fd, buf, BUFFSIZE)) < 0){
            if (errno == EINTR)
                goto again;
        }

#### 优化后可自动重启的系统调用
    无需代码层面认为干预 系统内部机制可自动重启被中断的系统调用
    自动重启的系统调用包括:
        ioctl read readv write writev 只有对低速设备操作时才会被信号中断
        wait waitpid 在捕捉到信号时总是被中断
    sigaction函数提供选项 是否自动重启系统调用

### 信号处理函数中哪些函数不能调用
    凡是可重入函数都可在信号处理函数中调用
#### 术语
    可重入:假设主流程中A函数执行 A函数被信号中断 指令流程发生跳转 转而执行信号处理函数 信号处理函数中调用A函数 信号处理函数中A函数重新执行完成 指令流程回到主流程A函数继续执行, 满足本条件不发生错误的函数成为可重入函数.

#### 常见不可重入函数
    函数内部使用静态数据结构
    函数调用malloc和free
    函数时标准I/O库函数

## 可靠信号术语和定义
    信号产生时,内核通常在进程表中以某种形式设置一个标志.当对信号采取动作时,我们说向进程递送了一个信号
    在信号产生(generation)和递送(delivery)之间的时间间隔内 称信号是未决的(pending)

    进程阻塞信号递送 且信号的动作是默认动作或者捕捉该信号 则进程对该信号保持未决状态
    进程对该信号解除阻塞 或者 对该信号的动作改为忽略 则进程对信号未非未决状态

    进程在信号递送给它之前仍可改变对信号的动作
    sigpending函数判定哪些信号是设置未阻塞并处于未决状态的

    信号屏蔽字signal mask 规定当前要阻塞递送到进程的信号集 对每种可能的信号 屏蔽字中有某一位与之对应
    sigprocmask函数检测和更改信号屏蔽字

## 发送信号
    int kill(pid_t pid, int signo) // 将信号发送给进程或者进程组
    int raise(int signo) // 进程向自身发送信号
    unsigned int alarm(unsigned int seconds) // 闹钟超时后发SIGALRM信号
    int pause() // 使调用进程挂起直至捕捉到一个信号 只有执行了一个信号处理程序并从其返回 pause才返回 这种情况下 pause返回-1 errno设置未EINTR

## sleep函数的正确实现

## 信号集相关API
    信号集是一种数据类型 不同信号的编号可能超过一个整型包含的位数因此定义信号集类型
    int sigemptyset(sigset_t *set) // 初始化set指向的信号集 清除其中的所有信号
    int sigfillset(sigset_t *set) // 初始化set指向的信号集 使其包括所有的信号
    int sigaddset(sigset_t *set) // 将一个信号添加到已有的信号集中
    int sigdelset(sigset_t *set) // 从信号集中删除一个信号
    int sigismember(const sigset_t *set, int signo) // 测试某信号是否属于信号集

    进程的信号屏蔽字规定当前阻塞而不能递送给进程的信号集
    sigprocmask函数可以检测 或 更改 或 同时进行检测和更改进程的信号屏蔽字
    // 检测 oset非空指针 当前信号屏蔽字通过oset返回
    // 更改 set非空指针 set是新的阻塞信号 how指定 阻塞集合取并集 替换为新的阻塞集合 解除集合中的信号阻塞
    // 检测和更改 oset和set都不为空指针
    // how值
    //      SIG_BLOCK    与当前信号屏蔽字取并集
    //      SIG_SETMASK  新集合替换当前信号屏蔽字
    //      SIG_UNBLOCK  与当信号屏蔽字取交集 解除这部分信号的阻塞
    // 本函数为单线程定义
    int sigprocmask(int how, const sigset_t *set, sigset_t *oset) // 

    // 返回调用进程中阻塞不能递送的信号集 通过set参数返回
    int sigpending(sigset_t *set)

## 检查或修改与指定信号相关联的处理动作
    // 本函数用来实现signal()函数
    // 检查 oact非空 则返回signo对应信号的上一个动作
    // 修改 act非空 则用act修改signo对应的信号的动作
    // 检查并修改 oact和act都非空
    int sigaction(int signo, const struct sigaction *act, struct sigaction *oact)
    struct sigaction {
        void (*sa_handler)(int); // 信号处理函数
        sigset_t sa_mask;  // 进程需要屏蔽的信号集
        int sa_flags; // 对信号处理的选项 如SA_RESTART由此信号中断的系统调用回自动重启
        void (*sa_sigaction)(int, siginfo_t*, void*); // 替代的信号处理程序
    }

## 对信号解除阻塞然后pause以等待以前被阻塞的信号发生
    // 在原子操作中先恢复信号屏蔽字 然后使进程休眠
    // 始终返回-1 并设置EINTR表示一个被中断的系统调用
    
    // 暂时用掩码给出的掩码替换调用进程的信号掩码 然后暂停进程直到
    // 传递一个信号 其动作是调用信号处理程序或终止进程
    // 如果信号终止进程 则sigsuspend()不会返回 如果信号被捕获 则sigsuspend()在信号之后返回
    // 处理程序返回 并且信号掩码恢复到调用sigsuspend()之前的状态
    int sigsuspend(const sigset_t *sigmask)

## 信号处理函数的应用
    // 默认
    #define SIG_DFL ((void(*)(int))0)
    // 忽略
    #define SIG_IGN ((void(*)(int))1)
### 服务器进程需要执行默认操作的信号
    SIGINT      终端中断符      默认终止
    SIGTERM     终止            默认终止
    SIGQUIT     终端退出符      默认终止+core
    SIGHUP      连接断开        默认终止
### 服务器进程需要执行忽略的信号
    SIGPIPE     往读端关闭的管道写      默认终止
    SIGCHLD     子进程状态改变          默认忽略
### 服务器不能阻塞屏蔽的信号 sigprocmask SIG_UNBLOCK
    SIGSEGV     无效内存引用        默认终止+core
    SIGBUS      硬件故障            默认终止+core
    SIGABRT     异常终止            默认终止+core
    SIGILL      非法硬件指令        默认终止+core
    SIGCHLD     子进程状态改变      默认忽略
    SIGFPE      算数异常            默认终止+core
### 用户定义信号
    SIGUSR1     默认终止
    SIGUSR2     默认终止

## nginx中信号的应用

