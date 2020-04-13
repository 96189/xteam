# 调试工具
## GDB

### 常用命令
* 条件断点

    break {line-or-function} if {expr}

* 内存值查看

    x/{n/f/u}   {addr}

        n n表示要显示的内存单元的个数
        f 表示显示的格式
            x 按十六进制格式显示变量
            d 按十进制格式显示变量
            u 按十进制格式显示无符号整型
            o 按八进制格式显示变量
            t 按二进制格式显示变量
            a 按十六进制格式显示变量
            i 指令地址格式
            c 按字符格式显示变量
            f 按浮点数格式显示变量
        u 表示一个地址单元的长度
            b 单字节 h 双字节 w 四字节 g 八字节

* 加载调试命令文件

    gdb -x {cmd.txt}

### gdb加载程序
* gdb {binfile}

    set args {param}

    show args

### core文件调试



