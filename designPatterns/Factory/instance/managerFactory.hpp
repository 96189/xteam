
// 简单工厂 - 静态方法工厂

#ifndef _MANAGER_FACTORY_HPP_
#define _MANAGER_FACTORY_HPP_

#include <map>
#include "workerConf.hpp"
#include "workerManager.hpp"


#define DEFAULT_MANAGER 0

class ManagerFactory 
{
private:
    std::map<int, WorkerManager*> managerMap_;
    std::map<int, WorkerConfig*> configMap_;

private:
    // 具体工厂创建方法
    WorkerManager* CreateWorkerManager(int id);
    WorkerManager* CreateWorkerManager(int type, WorkerConfig *pConfig);
public:
    // 构造 析构
    ManagerFactory();
    ~ManagerFactory();

public:
    // 应用添加 删除产品配置的方法
    void AddWorkerManagerConfig(int id, WorkerConfig* pConfig);
    void DeleteWorkerManagerConfig(int id);

    // 应用查询获取产品的方法
    WorkerManager* GetWorkerManager(int id);
    // 应用获取工厂实例的入口
    static ManagerFactory* Instance();
};

#endif