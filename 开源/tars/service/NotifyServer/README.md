
# NotifyServer #
## 协议 ##
    Notify.tars
```
    // 框架上报的信息 存储在数据库中
    void reportServer(string sServerName, string sThreadId, string sMessage);
    // 业务上报的告警信息 存储在g_notifyHash中
    void notifyServer(string sServerName, NOTIFYLEVEL level, string sMessage);
    // 上报框架信息或者业务告警信息
    void reportNotifyInfo(ReportInfo info);
    // 获取业务告警信息
    int getNotifyInfo(NotifyKey stKey, out NotifyInfo stInfo);
```
## NotifyServer ##
### 服务初始化 ###
```
void NotifyServer::initialize()
{
    // 实例化NotifyImp对象
    addServant<NotifyImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".NotifyObj");
    // 初始化hash表
    g_notifyHash = new TarsHashMap<NotifyKey, NotifyInfo, ThreadLockPolicy, MemStorePolicy>();
    // 初始化db线程
    _loadDbThread = new LoadDbThread();
    _loadDbThread->start();
}
```

* LoadDbThread线程

线程函数
```
void LoadDbThread::run()
{
    while (!_terminate)
    {
        loadData(); // 定期加载数据库表t_server_conf中固定数量的信息到_data内存缓存中
    }
}
```
线程对外提供的唯一接口
```
    /**
     * 获取set信息 从内存缓存中获取指定sAppName的信息
     */
    string getSetName(const string &sAppName)
    {
        map<string, string> &mData          = _data.getReader();
        map<string, string>::iterator it    = mData.find(sAppName);
        if (it != mData.end())
        {
            return it->second;
        }
        return "";
    }
```

* NotifyImp对象
```
void NotifyImp::reportServer(const string& sServerName, const string& sThreadId, const string& sResult, tars::TarsCurrentPtr current)
{
    info.sSet      = g_app.getLoadDbThread()->getSetName(sServerName + current->getIp());   // 通过db线程获取sAppName对应的信息
}
void NotifyImp::notifyServer(const string& sServerName, NOTIFYLEVEL level, const string& sMessage, tars::TarsCurrentPtr current)
{
    info.sSet     = g_app.getLoadDbThread()->getSetName(sServerName + current->getIp());    // 通过db线程获取sAppName对应的信息
}
```

* hashmap

    存储业务报警信息 方便读写

## QA ##
    服务读取的tars.tarsnotify.config.conf文件如何生成的?
    tars/framework/conf/目录下