
## 文本处理
* awk

* sed

## 拷贝与下载
* scp

* wget

## 接口测试
* curl

## 系统状态查询
* 机器核数查询

    逻辑核数 cat /proc/cpuinfo| grep "processor"| wc -l

    

## 进程状态查询
* 线程数查询

    cat /proc/{pid}/status  | grep Threads