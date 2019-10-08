
# PropertyServer属性统计服务 #

## 协议 ##
    Property.tars
    // 上报属性统计信息
    int reportPropMsg(map<StatPropMsgHead, StatPropMsgBody> statmsg);

## PropertyServer ##
### 服务初始化 ###
```
    void PropertyServer::initialize()
    {
        // 实例化属性上报处理对象
        addServant<PropertyImp>(ServerConfig::Application + "." + ServerConfig::ServerName +".PropertyObj" );
        // 初始化定时刷新数据线程
        _reapThread = new PropertyReapThread();
        _reapThread->start();
        // 双buffer初始化
        for(size_t i =0; i < vec.size(); ++i)
        {
            vec[i].first = 0;
            vec[i].second = 0;
        }
        _buffer[_selectBuffer] = vec;
        for(size_t i =0; i < vec.size(); ++i)
        {
            vec[i].first = 0;
            vec[i].second = 1;
        }
        _buffer[!_selectBuffer] = vec;
    }
```
* 属性上报处理对象PropertyImp

    实现Property.tars协议中定义的接口

功能: 将上报的数据存储到hash表中
```
int PropertyImp::reportPropMsg(const map<StatPropMsgHead,StatPropMsgBody>& propMsg, tars::TarsCurrentPtr current )
{
    handlePropMsg(propMsg, current);
}
int PropertyImp::handlePropMsg(const map<StatPropMsgHead, StatPropMsgBody> &propMsg, tars::TarsCurrentPtr current)
{
    for (map<StatPropMsgHead,StatPropMsgBody>::const_iterator it = propMsg.begin(); it != propMsg.end(); it++)
    {
        const StatPropMsgHead &head = it->first;
        const StatPropMsgBody &body = it->second;

        // 获取当前线程私有数据
        PropertyImpThreadData * td = PropertyImpThreadData::getData();
        if(td)
        {
            iIndex = td->_threadIndex;
        }

        // 检查是都扩张当前hash表
        int iHashKey = _hashf(sKey) % g_app.getBuffNum();
        PropertyHashMap *pHashMap = g_app.getHashMapBuff(iBufferIndex, iHashKey);
        float rate =  (pHashMap->getMapHead()._iUsedChunk) * 1.0/pHashMap->allBlockChunkCount();
        // hash表扩张
        if(rate > 0.9)
        {
            pHashMap->expand(pHashMap->getMapHead()._iMemSize * 2);
            TLOGERROR("PropertyImp::handlePropMsg hashmap expand to " << pHashMap->getMapHead()._iMemSize << endl);
        }

        // hash表新增数据
        int iRet = pHashMap->add(tHead, body);
    }
    return 0;
}
```

* 定时刷新数据线程PropertyReapThread

定时刷新函数

功能:定时刷新内存hash中存储的上报的属性信息到线程池队列中
```
void PropertyReapThread::run()
{
    // 线程循环开始之前的资源初始化 按需创建操作数据库的线程池
    int iInsertDataNum = PropertyDbManager::getInstance()->getInsertDbThreadNum();
    for(int i = 0; i < iInsertDataNum; ++i)
    {
        ReapSSDProcThread *r = new ReapSSDProcThread(this);
        r->start();
        _runners.push_back(r);
    }

    // 线程循环
    while (!_terminate)
    {
        try
        {
            //双buffer中一个buffer入库
            int iBufferIndex = !(g_app.getSelectBufferIndex());
            if(iBufferIndex != iLastIndex && g_app.getSelectBuffer(iBufferIndex, iInterval))
            {
                iLastIndex = iBufferIndex;

                // vector的每个元素是一个kv项
                // 创建dbNumber个kv项
                vector<PropertyMsg*> vAllPropertyMsg;
                for(int iStatIndex = 0; iStatIndex < dbNumber; ++iStatIndex)
                {
                    vAllPropertyMsg.push_back(new PropertyMsg());
                }

                // 从双buffer中获取数据到vAllPropertyMsg中的每一项
                getDataFromBuffer(iBufferIndex, vAllPropertyMsg, iTotalNum);

                // 双buffer中无数据
                if(iTotalNum <= 0)
                {
                    for(int iStatIndex = 0; iStatIndex < dbNumber; ++iStatIndex)
                    {
                        delete vAllPropertyMsg[iStatIndex];
                    }

                    vAllPropertyMsg.clear();
                }
                // 有数据待刷新到数据库中
                else
                {
                    QueueItem item;
                    map<string, vector<size_t> >& mIpHasDbInfo = PropertyDbManager::getInstance()->getIpHasDbInfo();
                    map<string, vector<size_t> >::iterator m_iter = mIpHasDbInfo.begin();

                    while(m_iter != mIpHasDbInfo.end())
                    {
                        vector<size_t> &vDb = m_iter->second;

                        for(size_t i = 0; i < vDb.size(); ++i)
                        {
                            int k = (i + TC_Common::strto<int>(sRandOrder)) % vDb.size();

                            item._index = vDb[k];
                            item._date  = sDate;
                            item._tflag = sFlag;
                            item._statmsg = vAllPropertyMsg[item._index];

                            // 选择线程
                            iInsertThreadIndex = PropertyDbManager::getInstance()->getDbToIpIndex(vDb[k]);
                            // 向线程队列中添加任务
                            _runners[iInsertThreadIndex]->put(item);
                        }

                        ++m_iter;
                    }
                }

                // 清空对应一段连续的hash内存数据
                for(int k = 0; k < g_app.getBuffNum(); ++k)
                {
                    PropertyHashMap *pHashMap = g_app.getHashMapBuff(iBufferIndex, k);
                    pHashMap->clear();
                }
            }
        }

        TC_ThreadLock::Lock lock(*this);
        timedWait(REAP_INTERVAL);
    }

    // 线程循环退出  线程函数终止之前的清理工作
    TLOGDEBUG("PropertyReapThread run setTerminateFlag true." << endl);
    PropertyDbManager::getInstance()->setTerminateFlag(true);
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

* 定时处理任务函数

功能:定时处理队列中的任务(刷新数据到数据库中)

```
void ReapSSDProcThread::run()
{
    while (!_bTerminate)
    {
        try
        {
            QueueItem item;
            if(pop(item))
            {
                if(item._statmsg != NULL)
                {
                    // 分表策略默认是按小时分表(创建24张表) 建表语句按照conf配置文件中的格式 数据库表为tars_property
                    PropertyDbManager::getInstance()->insert2MultiDbs(item._index, *item._statmsg, item._date, item._tflag);

                    delete item._statmsg;
                    item._statmsg = NULL;
                }
            }
        }
    }
}
```

### 双buffer机制 ###

### hash缓存机制 ###

