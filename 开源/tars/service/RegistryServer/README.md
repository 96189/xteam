# 服务注册中心 #
## 数据持久化与一致性 ##
    mysql
## 协议 ##
    QueryF.tars
    Registery.tars

## 服务查询/服务注册 ##
    QueryImp.h QueryImp.cpp
    RegistryImp.h RegistryImp.cpp

## RegistryServer ##
### 处理流程 ###
```
    {
        //加载registry对象的端口信息
        loadServantEndpoint(); 

        //ReapThread初始化时会用到
        // ...

        //全量和增量加载路由信息的线程
        _reapThread.init();
        _reapThread.start();

        //检查node超时的线程
        _checkNodeThread.init();
        _checkNodeThread.start();

        //监控所有服务状态的线程
        _checksetingThread.init();
        _checksetingThread.start();

        //异步处理线程
        _registryProcThread = new RegistryProcThread();
        int num = TC_Common::strto<int>(g_pconf->get("/tars/reap<asyncthread>", "3"));
        _registryProcThread->start(num);

        //供node访问的对象(实例化注册处理对象并存储地址信息)
        addServant<RegistryImp>((*g_pconf)["/tars/objname<RegistryObjName>"]);

        //供tars的服务获取路由的对象(实例化查询处理对象并存储地址信息)
        addServant<QueryImp>((*g_pconf)["/tars/objname<QueryObjName>"]);
        // ...
    }
```
* 定时操作线程ReapThread
    线程初始化
        _db.loadObjectIdCache 全量加载数据库相关信息到缓存(CDbHandle _objectsCache _mapServantStatus _setDivisionCache)中
    线程执行函数

    ```
    void ReapThread::run()
    {
        while(!_terminate)
        {
            _db.updateRegistryInfo2Db(_heartBeatOff);   // 定时刷新内存表_mapServantEndpoint中的对象-适配器(servant endpoint)到数据库中
            _db.loadObjectIdCache(_recoverProtect, _recoverProtectRate,_leastChangedTime2,true?false, false);   // 定时全量或者增量加载数据库相关信息到缓存(CDbHandle _objectsCache _mapServantStatus _setDivisionCache)中
            _db.checkRegistryTimeout(_registryTimeout);     // 定时更新心跳超时的主控状态为inactive(t_registry_info表)
        }
    }
    ```
* 监控tarsnode超时的线程CheckNodeThread
    线程执行函数
    ```
    void CheckNodeThread::run()
    {
        while(!_terminate)
        {
            if(tNow - tLastCheckNode >= _nodeTimeoutInterval)
            {
                _db.checkNodeTimeout(_nodeTimeout);
                tLastCheckNode = tNow;  // 定时更新心跳超时的tarsnode状态为set node.present_state='inactive', server.present_state='inactive', server.process_id=0(t_node_info表)
            }
        }
    }
    ```
* 监控所有服务状态的线程CheckSettingState
    线程执行函数
    ```
    void CheckSettingState::run()
    {
        while(!_terminate)
        {
            if(tNow - tLastQueryServer >= _checkingInterval)
            {
                _db.checkSettingState(_leastChangedTime);   // 定时检查t_server_conf并启动服务状态标记为active但未启动(根据registry_timestamp判断)的服务
            }
        }
    }
    ```
* 异步处理命令的线程池RegistryProcThread
    线程执行函数
    ```
    void RegistryProcThreadRunner::run()
    {
        while (!_terminate)
        {
            if(_proc->pop(info) && !_terminate)
            {
                if(info.cmd == EM_NODE_KEEPALIVE)
                {
                    _db.keepAlive(info.nodeName,info.loadinfo); // 更新心跳和节点状态set last_heartbeat=now(), present_state='active'(t_node_info表)
                }
                else if(info.cmd == EM_UPDATEPATCHRESULT)
                {
                    _db.setPatchInfo(info.appName,info.serverName,info.nodeName,info.patchVersion,info.patchUserName);  // 更新指定服务的服务patch相关的信息patch_version patch_user patch_time(t_server_conf表)
                }   
                else if(info.cmd == EM_REPORTVERSION)
                {
                    _db.setServerTarsVersion(info.appName, info.serverName, info.nodeName, info.tarsVersion);   // 更新指定服务的tars_version(t_server_conf表)
                }
                else
                {
                    
                }
            }
        }
    }
    ```
### 数据读写核心 ###
    DbHandle.h DbHandle.cpp

