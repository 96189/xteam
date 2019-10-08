
# 管理控制服务 #
## 协议 ##
    Node.tars
    AdminReg.tars

## 服务管理 ##
    AdminRegistryImp.h  AdminRegistryImp.cpp

## AdminRegistryServer ##
### 处理流程 ###
```
    {
        // 加载本服务路由信息到内存
        loadServantEndpoint();

        // 轮询线程,定时刷新内存中路由信息到数据库,定时更新组规则
        _reapThread.init();
        _reapThread.start();

        // 供admin client访问的对象,实例化处理管理命令的对象并加载路由信息到内存
        string adminObj = g_pconf->get("/tars/objname<AdminRegObjName>", "");
        addServant<AdminRegistryImp>(adminObj);
    }
```
* 定时轮询线程AdminReapThread
```
void AdminReapThread::run()
{
    while(!_terminate)
    {
        {
            if(tNow - tLastUpdateTime >= _updateInterval)
            {
                _db.updateRegistryInfo2Db(_heartBeatOff);   // 定时用内存数据_mapServantEndpoint刷新数据库表t_registry_info的servant endpoint字段
                _db.loadIPPhysicalGroupInfo();
            }

            // 轮询心跳超时的主控
            if(tNow - tLastQueryServer >= _timeout)
            {
                _db.checkRegistryTimeout(_timeout);     // 定时刷新数据库表t_registry_info心跳超时节点状态为present_state='inactive'
            }
        }
    }
}
```
### 处理命令的对象实例化 ###
    AdminRegistryImp.h  AdminRegistryImp.cpp
    ExecuteTask.h   ExecuteTask.cpp
    管理的相关接口定义在AdminReg.tars

### 数据读写核心 ###
    DbHandle.h DbHandle.cpp