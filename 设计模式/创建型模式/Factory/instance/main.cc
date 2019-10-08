
#include "workerConf.hpp"
#include "workerManager.hpp"
#include "managerFactory.hpp"

int main(int argc, char const *argv[])
{
    WorkerConfig *pConfig = NULL;
    int id = 1;
    int cmd = 0; // UPDATE_USE_DEFAULT

    // 简单工厂-静态方法工厂-实例
    ManagerFactory *pManagerFactory = ManagerFactory::Instance();
    // 添加任务id和任务配置-内部生成任务管理对象(任务管理对象持有工厂方法模式的接口类型变量)
    pManagerFactory->AddWorkerManagerConfig(id, pConfig);
    // 任务管理-工厂方法模式-获得id对应的任务管理对象-若对象不存在则创建
    WorkerManager *pManager = pManagerFactory->GetWorkerManager(id);
    // 具体的任务管理对象来处理数据包
    // 此处任务管理对象内部根据cmd创建不同的处理器 由具体的处理器来执行数据包处理
    pManager->ProcessClientPacket(cmd);
    return 0;
}
