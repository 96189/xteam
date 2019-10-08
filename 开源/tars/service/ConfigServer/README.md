
# ConfigServer #
## 协议 ##
    ConfigF.tars
## ConfigServer ##
### 功能 ##
```
void ConfigServer::initialize()
{
    // 实例化ConfigImp对象 并添加到全局单例ServantHelperManager的_servant_creator表中
    addServant<ConfigImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".ConfigObj");
}
```

### ConfigImp ###
    实现ConfigF.tars中的接口
    TC_Mysql _mysqlConfig;持有数据库连接操作t_config_files表

