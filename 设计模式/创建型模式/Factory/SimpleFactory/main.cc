
#include "oldprocessMsg.hpp"
#include "processMsg.hpp"

// switch cash 结构的消息处理入口测试
void TestOldProcessMsg()
{
    Msg msg(LOGIN, "login");
    OldProcessMsg(&msg);
}

// 使用简单工厂重构之后的 消息处理入口测试
void TestProcessMsg()
{
    Msg msg(LOGIN, "login");
    ProcessMsg(&msg);
}

int main(int argc, char const *argv[])
{
    TestOldProcessMsg();
    TestProcessMsg();
    return 0;
}
