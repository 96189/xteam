本部分通过升序链表作容器管理定时器，实现简单的定时踢掉空闲连接的功能(或者说是简单的心跳机制实现)

定时器操作相关接口进行了简单封装,网络部分基本使用原始api,尚未进行封装.i/o事件和定时器事件统一通过epoll处理.
在此基础上实现了echo、chat、transfile三种功能，尽可能的分离业务层代码和网络层代码。

Test:
Echo和Chat服务连接上若60秒内无数据,则被定时器超时关掉连接.TransFile为300秒
测试Echo
./Server -h 192.168.153.168 -p 9898 -e
nc -v 192.168.153.168 9898

测试Chat
./Server -h 192.168.153.168 -p 9898 -c
nc -v 192.168.153.168 9898

测试TransFile
./Server -h 192.168.153.168 -p 9898 -t 
nc 192.168.153.168 9898
get filename

以上3中功能的实现都是基于字节流协议,尚未自定义协议,消息的边界依赖'\0'字符,因此也未实现消息的编解码(codec)
每个连接配置一个缓冲区,由vector<char>实现。
TimerObj 
exptime_使用绝对时间检查超时,精度为毫秒.超时执行回调关闭连接.

TimerManager
TimerObj的链表,提供CheckExpired、StartTimer、StopTimer接口管理定时器.

client_data
每一个连接配置一个缓冲区buf_,定时器timer_,并存储fd和地址.

ClientMap管理连接

自定义协议替换掉简单的字节流后无法用nc测试
需要编译client.cc后测试
g++ client.cc net.cc chatMessage.pb.cc -g -std=c++0x -o client -lprotobuf -luuid -lpthread
编译filename.proto文件
protoc -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/filename.proto
std::thread 需要c++0x选项