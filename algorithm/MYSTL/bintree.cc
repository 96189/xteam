#include <stdio.h>
#include "binnode.h"

using namespace MYSTL;

// 前序序列
int preOrder[] = { 5, 2, 1, 3, 9, 7, 11 };
int prelen = sizeof(preOrder) / sizeof(preOrder[0]);
// 中序序列
int inOrder[] = { 1, 2, 3, 5, 7, 9, 11 };
int inlen = sizeof(inOrder) / sizeof(inOrder[0]);
// 后序序列
int postOrder[] = { 1, 3, 2, 7, 11, 9, 5 };
int postlen = sizeof(inOrder) / sizeof(inOrder[0]);


template <typename T>
class Iterator 
{
public:
    void operator()(BinNodePosi(T) p)
    {
        printf("addr:%p parent:%p data:%d lChild:%p rChild:%p height:%d\n", p, p->parent,p->data,p->lChild,p->rChild,p->height);
    }
};


#include "bintree.h"
void TestBinTree()
{
    BinTree<int> bt;
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
}

#include "binSearchTree.h"
void TestBinSearchTree()
{
    BST<int> bst;
    bst.buildTreePosIn(postOrder, postlen, inOrder, inlen);

    Iterator<int> iter;
    // 先序遍历
    bst.travPre(iter);
    printf("\n");
    assert(bst.search(9)->data == 9);
    assert(!bst.search(19));
    assert(bst.size() == 7);
    assert(bst.insert(19)->data == 19 && bst.insert(19)->parent->data == 11);
    bst.travPre(iter);
    printf("\n");
    bst.remove(19);
    bst.travPre(iter);
    printf("\n");
    bst.remove(1);
    bst.travPre(iter);
    printf("\n");
    bst.remove(5);
    bst.travPre(iter);
    printf("\n");
}

#include "avlTree.h"
void TestAvlTree()
{
    Iterator<int> iter;
    AVL<int> avl;
    for (int i = 0; i < inlen; ++i)
    {
        avl.insert(inOrder[i]);
        // 先序遍历
        avl.travPre(iter);
        printf("\n");
    }
    assert(avl.size() == 7);
    avl.remove(1);
    avl.remove(2);
    avl.remove(3);
    printf("\n");
    avl.travPre(iter);
    printf("\n");
}

#include "splayTree.h"
void TestSplayTree()
{
    Iterator<int> iter;
    Splay<int> spt;
    for (int i = 1; i <= 7; ++i)
    {
        spt.insert(i);
    }
    spt.travPre(iter);
    printf("\n");
    assert(spt.root()->data == 7);
    for (int i = 1; i <= 1; ++i)
    {
        spt.search(i);
    }
    spt.travPre(iter);
    printf("\n");    
    spt.search(8);
    assert(spt.root()->data == 7);
    spt.remove(3);
    assert(spt.root()->data == 4);
    spt.search(5);
    assert(spt.root()->data == 5);
}

#include "B-Tree.h"
void TestBTree()
{
    BTree<int> bt;
}

int main(int argc, char* argv[])
{
    // TestBinTree();
    // TestBinSearchTree();
    // TestAvlTree();
    // TestSplayTree();
    TestBTree();
    return 0;
}
