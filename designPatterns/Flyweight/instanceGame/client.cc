
#include "flyweightFactory.h"
#include <vector>

int main(int argc, char* argv[])
{
    std::vector<Tree*> treeVec;
    TreeFactory *pTrees = TreeFactory::getInstance();
    treeVec.push_back(pTrees->getTree(std::string("Oak"), {1, 5}, {3}, {5}, {7}, {9}));
    treeVec.push_back(pTrees->getTree(std::string("Spruce"), {18, 7}, {2}, {4}, {7}, {3}));
    treeVec.push_back(pTrees->getTree(std::string("Birch"), {14, 6}, {3}, {6}, {8}, {9}));
    treeVec.push_back(pTrees->getTree(std::string("Jungle"), {11, 2}, {9}, {5}, {1}, {8}));
    treeVec.push_back(pTrees->getTree(std::string("Acacia"), {4, 9}, {5}, {2}, {9}, {3}));
    treeVec.push_back(pTrees->getTree(std::string("Aster"), {3, 7}, {6}, {1}, {8}, {3}));

    // int n = 10;
    // for (size_t i = 0; i < n; ++i)
    // {
    //     treeVec.push_back(pTrees->getTree(std::string("Oak"), {x, x}, {x}, {x}, {x}, {x}));
    // }
    // ...

    // 7种类型的tree 每种类型的tree有n+1棵  但是n+1棵树只有7种树模型对象
    // 树模型是本例中可共享的内部状态 每种类型只存在一个实例对象
    // 树的位置 高度 颜色等属性是不可共享的由客户端改变的外部属性
    return 0;
}