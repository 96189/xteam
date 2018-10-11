
//

#include <assert.h>
#include "singleton.hpp"
#include "managerFactory.hpp"
#include "defaultWorkerManager.hpp"

#include <iostream>

// 构造 析构
ManagerFactory::ManagerFactory()
{

}
ManagerFactory::~ManagerFactory()
{

}


// 具体工厂创建方法
WorkerManager *ManagerFactory::CreateWorkerManager(int id)
{
    WorkerManager *pManager = NULL;
    std::map<int, WorkerConfig *>::iterator it = configMap_.find(id);
    // 存在配置 则创建
    if (it != configMap_.end())
    {
        int type = DEFAULT_MANAGER;
        pManager = CreateWorkerManager(type, it->second);
        if (pManager)
        {
            managerMap_[id] = pManager;
        }
    }
    else 
    {
        // log
        std::cout << "CreateWorkerManager no exists config" << std::endl;
    }
    // 否则 返回NUNLL

    return pManager;
}
WorkerManager *ManagerFactory::CreateWorkerManager(int type, WorkerConfig *pConfig)
{
    WorkerManager *pManager = NULL;
    switch (type)
    {
    case DEFAULT_MANAGER:
        std::cout << "ManagerFactory CreateWorkerManager DEFAULT_MANAGER" << std::endl;
        pManager = new DefaultWorkerManager(pConfig);
        break;
    default:
        // log
        break;
    }
    return pManager;
}

// 应用添加 删除产品配置的方法
void ManagerFactory::AddWorkerManagerConfig(int id, WorkerConfig *pConfig)
{
    // assert(id == pConfig->id);
    configMap_[id] = pConfig;
    GetWorkerManager(id);
}
void ManagerFactory::DeleteWorkerManagerConfig(int id)
{

}

// 应用查询获取产品的方法
WorkerManager *ManagerFactory::GetWorkerManager(int id)
{
    WorkerManager *pManager = NULL;
    std::map<int, WorkerManager *>::iterator it = managerMap_.find(id);
    // 查询不到 则创建
    if (it == managerMap_.end())
    {
        pManager = CreateWorkerManager(id);
    }
    else
    {
        pManager = it->second;
    }
    return pManager;
}

// 应用获取工厂实例的入口
ManagerFactory *ManagerFactory::Instance()
{
    return Singleton<ManagerFactory>::Instance();
}