
#include "composite.h"
#include "leaf.h"

int main(int argc, char* argv[])
{
    NodeTree tree;      // 叶子节点
    NodePlayer player;  // 玩家节点
    NodeRoot root;      // 组合节点

    root.addChild(&tree);
    root.addChild(&player);

    for (;;)
    {
        root.cycle();   // 刷新
        root.draw();    // 渲染
    }
    return 0;
}