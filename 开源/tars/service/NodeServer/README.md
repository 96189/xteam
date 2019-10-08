
## 协议 ##
    AdminF.tars
    Node.tars
    NodeF.tars
    QueryF.tars

## NodeServer ##
### 处理流程 ###
```
{
    // 初始化主控AdminProxy(单例)的名字 配置文件中字段QueryObj后的内容(直接调用内容为ip时用""代替)
    initRegistryObj();

    // 实例化NodeObj和ServerObj对象 并添加到全局单例ServantHelperManager的_servant_creator表中
    addServant<NodeImp>(sNodeObj);
    addServant<ServerImp>(sServerObj);

    // 获取NodeAdapter配置的ip
    g_sNodeIp = getAdapterEndpoint("NodeAdapter").getHost();

    // 保活线程
    _keepAliveThread   = new KeepAliveThread();
    _keepAliveThread->start();

    // 各服务内存用量上报线程
    _reportMemThread = new ReportMemThread();
    _reportMemThread->start();

    // 批量发布线程
    _batchPatchThread  = new BatchPatch();
    _batchPatchThread->start(iThreads);
    g_BatchPatchThread  = _batchPatchThread;

    // 日志删除线程 删除目标取决于CommandDestroy向RemoveLogManager的_reqQueue中添加的任务
    _removeLogThread = new RemoveLogManager();
    _removeLogThread->start(iThreads);
    g_RemoveLogThread = _removeLogThread;
}
```
* 保活线程KeepAliveThread
```
void KeepAliveThread::run()
{
    while (!_terminate)
    {
            // 获取主控代理
            if (!_registryPrx)
            {
                _registryPrx = AdminProxy::getInstance()->getRegistryProxy();
            }

            // 注册node信息
            if (_registryPrx && bRegistered == false)
            {
                bRegistered = registerNode();
            }

            // 加载服务
            if (bLoaded == false)
            {
                bLoaded = loadAllServers();
            }

            // 检查服务的limit配置是否需要更新
            if (bLoaded)
            {
                ServerFactory::getInstance()->setAllServerResourceLimit();
            }

            // 检查服务(服务未开启且配置自动开启则运行服务)
            checkAlive();

            // 上报node状态
            if (reportAlive() != 0)
            {
                bRegistered = false; //registry服务重启  需要重新注册
            }
    }
}
```
* 内存用量上报线程ReportMemThread
```
void ReportMemThread::run()
{
    while (!_shutDown)
    {
            report();   // 定时上报每个服务的实例内存使用情况 通过读取"/proc/" + spid + "/statm"内容获取内存使用状况
    }
}
```
* 批量发布线程对象BatchPatch
    创建批量发布线程池
    ```
    for (int i = 0; i < iNum; i++)
    {
        BatchPatchThread * t = new BatchPatchThread(this);
        t->setPath(_downloadPath);
        t->start();
        _runners.push_back(t);
    }
    ```
    发布线程BatchPatchThread
    ```
    void BatchPatchThread::run()
    {
        while (!_shutDown)
        {
            if (_batchPatch->pop_front(item))   // 从线程池发布队列中获取任务 任务数据来源为NodeImp::patchPro
            {
                doPatchRequest(item.first, item.second); // 处理任务(构造发布命令对象 发布)  已发布任务的拉起依赖KeepAliveThread定时检查拉起
            }
        }
    }
    ```
* 日志删除对象RemoveLogManager
    创建日志删除线程池
    ```
    void RemoveLogManager::start(int iNum)
    {
        for (int i = 0; i < iNum; i++)
        {
            RemoveLogThread * t = new RemoveLogThread(this);
            t->start();
            _runners.push_back(t);
        }
    }
    ```
    日志删除线程
    ```
    void RemoveLogThread::run()
    {
        while (!_shutDown)
        {
            // 数据来源 NodeImp::destroyServer CommandDestroy command(pServerObjectPtr) g_RemoveLogThread->push_back
            if (_manager->pop_front(sLogPath))  
            {
                if (TC_File::isFileExistEx(sLogPath, S_IFDIR))
                {
                    int ret = TC_File::removeFile(sLogPath,true);   // 删除被销毁的服务的日志目录
                }
            }
        }
    }
    ```

* NodeImp对象
    实现Node.tars协议中定义的接口

* ServerImp对象
    实现NodeF.tars协议中定义的接口

### 设计模式-命令模式 ###
TODO