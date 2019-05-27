
# 发布服务 #
## 协议 ##
    Patch.tars
## PatchServer ##
### 处理流程 ###
```
    // 实例化发布处理对象
    addServant<PatchImp>(ServerConfig::Application + "." + ServerConfig::ServerName +".PatchObj");
```
### PatchImp ###
    实现Patch.tars定义的接口
    从文件下载或者从文件内存缓存下载