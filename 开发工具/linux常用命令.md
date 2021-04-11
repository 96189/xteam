
* 文本处理
    * awk
    * sed

* 拷贝与下载
    * scp
    * wget

* 接口测试
    * curl

* 进程状态查询
    * ps
    * pstack
    * ptree
    * lsof

* 调试命令
    * gdb
    * strace

* 网络状态查询
    * netstat
    * ss

* 机器状态查询
    * top
    * free
    * iotop
    * iostat
    * pidstat

* 磁盘状态和占用查询
    * df
    * du

## 系统状态查询
* 机器核数查询

    逻辑核数 cat /proc/cpuinfo| grep "processor"| wc -l

    

## 进程状态查询
* 线程数查询

    cat /proc/{pid}/status  | grep Threads