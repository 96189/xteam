# QueryServer #
## 协议 ##
### 协议类型 ###
    json
### 协议自定义方法 ###
    QUERY   数据查询
    TIMECHECK   查看最后入库时间
### 协议解析器 ###
    RequestDecoder

## QueryServer ##
### 服务初始化 ###
```
void  QueryServer::initialize()
{
    // 初始化定时检测线程更新存活的数据库集合
    _dBThread = new DBThread();
    _dBThread->start();

    // 时间查询相关初始化
    // 最后入库时间timecheck检查线程池
    _timeCheck.init(iTimeCheckPoolSize);
    _timeCheck.start();

    // 数据查询相关初始化
    // 并发查询线程池(提高一个请求查询多个数据库的效率)
    _poolDb.init(iDbThreadPoolSize);
    _poolDb.start();
    // 查询任务query处理线程(多个请求并发排队)
    _tpoolQueryDb = new QueryDbThread();
    _tpoolQueryDb->start(iQueryDbPoolSize);

    // 实例化查询请求处理对象 关联协议解析器
    addServant<QueryImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".NoTarsObj");
    addServantProtocol(ServerConfig::Application + "." + ServerConfig::ServerName + ".NoTarsObj", &JsonProtocol::parse);
}
```

### 定时检查存活数据库集合线程DBThread ##
```
void DBThread::run()
{
    while (!_bTerminate)
    {
        tryConnect();   // 尝试连接存活数据库集合中的每一个数据库 更新当前存活的数据库集合 对异常的报警

        TC_ThreadLock::Lock lock(*this);
        timedWait(REAP_INTERVAL);
    }
}
```

### 查询请求处理对象QueryImp ###

* 查询入口

```
int QueryImp::doQuery(const string sUid, const string &sIn, bool bTarsProtocol, tars::TarsCurrentPtr current)
{
    try
    {
        // 消息解码
        RequestDecoder decoder(s);
        int ret = decoder.decode();
        // 查看最后入库时间
        if(ret == RequestDecoder::TIMECHECK)
        {
            // 最后一次入库时间查询(由线程池对多个数据库发起查询 提高效率)
            string lasttime = _proxy.getLastTime(decoder.getSql());
            sResult += "lasttime:" + lasttime + "\n";
            sResult += "endline\n";

            // // 查询结果立即响应
            current->sendResponse(sResult.c_str(), sResult.length());
        }

        // 数据查询
        else if(ret == RequestDecoder::QUERY)
        {
            // 构造任务对象
            QueryItem * pItem = new QueryItem();
            pItem->sUid     = sUid;
            pItem->current  = current;
            pItem->mQuery   = decoder.getSql();
            pItem->bFlag = true;

            // 向线程池队列中添加任务
            g_app.getThreadPoolQueryDb()->put(pItem);

            // 查询结果由处理线程响应
        }
    }
    return 0;
}

```

* 时间查询线程池TC_ThreadPool
```
// 时间查询接口
string DbProxy::getLastTime(const map<string,string>& mSqlPart)
{
    try
    {
        // 获取当前存活的所有数据库
        vector<TC_DBConf> vDbInfo = g_app.getAllActiveDbInfo();
        int iThreads = vDbInfo.size();
        if(iThreads > 0)
        {
            // 多个数据库的结果集合
            vector<string> res(iThreads);
            // 构造查询参数
            _queryParam._run_times = iThreads;

            // 多个数据库用线程池来提高查询效率
            // 遍历数据库对象集合
            for (int i=0; i< iThreads; i++)
            {
                const string tbname = mSqlPart.find("dataid")->second;
                // 对当前数据构造查询任务
                auto fwrapper = std::bind(&selectLastMinTime,
                                           std::cref(sUid),
                                           i,
                                           std::cref(tbname),
                                           std::cref(vDbInfo[i]),
                                           std::ref(res[i]),
                                           std::ref(_queryParam));

                // 添加查询任务到_timecheck线程池
                g_app.getThreadPoolTimeCheck().exec(fwrapper);
            }
            // 等待所有数据库的_timecheck都被执行完成 每次等待300ms 尝试10次
            while(_queryParam._atomic.get() != _queryParam._run_times)
            {
                {
                    TC_ThreadLock::Lock lock(_queryParam._monitor);
                    rc = _queryParam._monitor.timedWait(300);
                }
                ++ifail;
                if(!rc)
                {
                    if(ifail >= 10) break;
                }
            }

            // 获取多个数据库中入库时间最小的时间
            if(rc)
            {
                for(int i = 0; i < iThreads; ++i)
                {
                    if(res[i] < min)
                    {
                        min = res[i];
                    }
                }
            }
        }
    }

    // 重置查询参数
    _queryParam._run_times = 0;
    _queryParam._run_result = 0;
    _queryParam._atomic = 0;

    return min;
}
```

* 数据查询线程QueryDbThread

数据查询入口

```
void HandleThreadRunner::run()
{
    while (!_terminate)
    {
        // 从任务队列中取出任务
        if(!_terminate && _proc->pop(pQueryItem))
        {
            // 执行任务(本次任务由线程池对多个数据库查询提高查询效率)
            _dbproxy.queryData(pQueryItem->mQuery, sRes, pQueryItem->bFlag);

            // 响应查询结果
             pQueryItem->current->sendResponse(sRes.c_str(), sRes.length());
        }
    }
}

```

线程池提高多个数据库查询效率
```
void DbProxy::queryData(map<string, string> &mSqlPart, string &sResult, bool bDbCountFlag)
{
    try
    {
        vector<TC_DBConf> vActive;
        // 获取当前存活的数据库集合
        vActive = g_app.getActiveDbInfo();
        if(iThreads > 0)
        {
            vector<string> res(iThreads);
            vector<map<string, vector<Int64> > >  vDataList(iThreads);
            // 查询参数构造
            _queryParam._run_times = iThreads;

            // 对每个数据库构造任务
            for(int i=0; i < iThreads; i++)
            {
                // 构造任务
                auto fwrapper = std::bind(&query,
                                          i,
                                          std::cref(vActive[i]),
                                          std::ref(mSqlPart),
                                          std::ref(vDataList[i]),
                                          std::ref(res[i]),
                                          std::ref(_queryParam));
                // 添加任务到线程池中
                 g_app.getThreadPoolDb().exec(fwrapper);
            }
            // 等待添加到线程池中的所有数据库数据查询任务都被完成
            while(_queryParam._atomic.get() != _queryParam._run_times)
            {
                {
                    TC_ThreadLock::Lock lock(_queryParam._monitor);
                    rc = _queryParam._monitor.timedWait(6000);
                }
                ++ifail;
                if(!rc)
                {
                    if(ifail >= 10) break;
                }
            }
            if(rc)
            {
                string sLasttime = getLastTime(mSqlPart);
                // 构造查询结果
                createRespHead(res, sLasttime, sHead, bDbCountFlag);
                createRespData(sUid, mSqlPart, vDataList, sHead, sResult);
            }
        }

    }

    // 重置查询参数
    _queryParam._run_times = 0;
    _queryParam._run_result = 0;
    _queryParam._atomic = 0;
}
```


### 

## 其他 ##
### 配置文件路径 ###
    tars/framework/deploy