#include "bintree.h"
#include <stdio.h>
#include "binnode.h"

using namespace MYSTL;

template <typename T>
class Iterator 
{
public:
    void operator()(BinNodePosi(T) p)
    {
        printf("%d ", p->data);
    }
};

int main(int argc, char* argv[])
{
    MYSTL::BinTree<int> bt;
    // 前序序列
    int preOrder[] = { 5, 2, 1, 3, 9, 7, 11 };
    int prelen = sizeof(preOrder) / sizeof(preOrder[0]);
    // 中序序列
    int inOrder[] = { 1, 2, 3, 5, 7, 9, 11 };
    int inlen = sizeof(inOrder) / sizeof(inOrder[0]);
    // 后序序列
    int postOrder[] = { 1, 3, 2, 7, 11, 9, 5 };
    int postlen = sizeof(inOrder) / sizeof(inOrder[0]);
    // 前序 中序建树
    // bt.buildTreePreIn(preOrder, prelen, inOrder, inlen);
    // 后序 中序建树
    bt.buildTreePosIn(postOrder, postlen, inOrder, inlen);

    Iterator<int> iter;
    // 先序遍历
    bt.travPre(iter);
    printf("\n");
    // 中序遍历
    bt.travIn(iter);
    printf("\n");
    // 后序遍历
    bt.travPost(iter);
    printf("\n");
    // 层次遍历
    bt.travLevel(iter);
    printf("\n");
    // 节点个数
    assert(bt.size() == 7 && !bt.empty());
    return 0;
}