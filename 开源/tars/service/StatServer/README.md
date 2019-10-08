
# StatServer #
## 协议 ##
    StatF.tars
    ```
    // 上报模块间调用信息
    int reportMicMsg(map<StatMicMsgHead,StatMicMsgBody> msg, bool bFromClient);
    // 上报模块间调用采样信息
    int reportSampleMsg(vector<StatSampleMsg> msg);
    ```

## StatServer ##
### 服务初始化 ###
    ```
    void StatServer::initialize()
    {
        // 实例化调用上报处理对象
        addServant<StatImp>( ServerConfig::Application + "." + ServerConfig::ServerName +".StatObj" );
        // 初始化双buffer
        for(size_t i =0; i < vec.size(); ++i)
        {
            vec[i] = 0;
        }
        _vBuffer[0]=vec;
        _vBuffer[1]=vec;

        // 初始化定时刷新线程
        _pReapSSDThread = new ReapSSDThread();
        _pReapSSDThread->start();
    }
    ```
* 状态上报处理对象StatImp

    实现StatF.tars协议接口

```
int StatImp::reportMicMsg( const map<tars::StatMicMsgHead, tars::StatMicMsgBody>& statmsg,bool bFromClient, tars::TarsCurrentPtr current )
{
    for (map<StatMicMsgHead, StatMicMsgBody>::const_iterator it = statmsg.begin(); it != statmsg.end(); it++)
    {
        // kv上报数据添加到hashmap
        int iAddHash        = addHashMap(head,body);
    }
    return 0;
}
```

* 定时刷新线程ReapSSDProcThread

功能:定时刷新hashmap中的数据到指定线程的队列中

线程处理函数
```
void ReapSSDThread::run()
{
    // 创建线程池
    for(int i = 0; i < iInsertDataNum; ++i)
    {
        ReapSSDProcThread *r = new ReapSSDProcThread(this);
        r->start();
        _runners.push_back(r);
    }

    // 线程函数
    while (!_terminate)
    {
        try
        {
            //双buffer中一个buffer入库
            int iBufferIndex = !(g_app.getSelectBufferIndex());
            if(iBufferIndex != iLastIndex && g_app.getSelectBuffer(iBufferIndex, iInterval))
            {
                // 创建dbNumber个kv对象
                vector<StatMsg*> vAllStatMsg;
                for(int iStatIndex = 0; iStatIndex < dbNumber; ++iStatIndex)
                {
                    vAllStatMsg.push_back(new StatMsg());
                }

                // 从hashmap中获取kv数据对象到vAllStatMsg 实际获取的个数为iTotalNum
                getDataFromBuffer(iBufferIndex, vAllStatMsg, iTotalNum);

                // hashmap中没有数据
                if(iTotalNum <= 0)
                {
                    for(int iStatIndex = 0; iStatIndex < dbNumber; ++iStatIndex)
                    {
                        delete vAllStatMsg[iStatIndex];
                    }
                    vAllStatMsg.clear();
                }
                // 从hashmap中得到iTotalNum个kv数据
                else
                {
                    map<string, vector<size_t> >& mIpHasDbInfo = StatDbManager::getInstance()->getIpHasDbInfo();
                    map<string, vector<size_t> >::iterator m_iter = mIpHasDbInfo.begin();
                    while(m_iter != mIpHasDbInfo.end())
                    {
                        vector<size_t> &vDb = m_iter->second;

                        for(size_t i = 0; i < vDb.size(); ++i)
                        {
                            int k = (i + TC_Common::strto<int>(sRandOrder)) % vDb.size();
                            // 构建任务数据
                            item._index = vDb[k];
                            item._date  = sDate;
                            item._tflag = sFlag;
                            item._statmsg = vAllStatMsg[item._index];

                            // 选择线程
                            iInsertThreadIndex = StatDbManager::getInstance()->getDbToIpIndex(vDb[k]);
                            // 向选定的线程的任务队列中添加任务
                            _runners[iInsertThreadIndex]->put(item);
                        }
                        ++m_iter;
                    }
                }

                // 清空指定hash表
                for(int k = 0; k < g_app.getBuffNum(); ++k)
                {
                    StatHashMap *pHashMap = g_app.getHashMapBuff(iBufferIndex, k);
                    pHashMap->clear();
                }
            }
        }
        TC_ThreadLock::Lock lock(*this);
        timedWait(REAP_INTERVAL);
    }

    // 清理线程池
    for(size_t i = 0; i < _runners.size(); ++i)
    {
        if(_runners[i]->isAlive())
        {
            _runners[i]->terminate();

            _runners[i]->getThreadControl().join();
        }
    }
    for(size_t i = 0; i < _runners.size(); ++i)
    {
        if(_runners[i])
        {
            delete _runners[i];
            _runners[i] = NULL;
        }
    }
}
```
* 任务处理线程ReapSSDProcThread

功能:处理任务队列中的任务(写调用信息到数据库)

线程处理函数
```
    void ReapSSDProcThread::run()
    {
        while (!_terminate)
        {
            QueueItem item;
            if(pop(item))
            {
                if(item._statmsg != NULL)
                {

                    // 按小时分表(24张表) 建表语句按照conf配置文件中的格式 数据库为tars_stat
                    StatDbManager::getInstance()->insert2MultiDbs(item._index, *item._statmsg, item._date, item._tflag);
                    delete item._statmsg;
                    item._statmsg = NULL;
                }
            }
        }
    }
```

