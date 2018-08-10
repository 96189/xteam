#include "bintree.h"
#include <stdio.h>

template <typename T>
class Iterator 
{
public:
    void operator()(T& e)
    {
        printf("%d ", e);
    }
};

int main(int argc, char* argv[])
{
    MYSTL::BinTree<int> bt;
    int preOrder[] = { 5, 2, 1, 3, 9, 7, 11 };
    // int preOrder[] = { 2, 1, 3 };
    int prelen = sizeof(preOrder) / sizeof(preOrder[0]);
    int inOrder[] = { 1, 2, 3, 5, 7, 9, 11 };
    // int inOrder[] = { 1, 2, 3 };
    int inlen = sizeof(inOrder) / sizeof(inOrder[0]);
    // int postOrder[] = { 1, 3, 2, 7, 11, 9, 5 };
    // int postlen = sizeof(inOrder) / sizeof(inOrder[0]);
    // bt.buildTreePreIn(preOrder, 0, prelen - 1, inOrder, 0, inlen - 1);
    bt.buildTreePreIn(preOrder, prelen, inOrder, inlen);
    Iterator<int> iter;
    bt.travPre(iter);
    printf("\n");
    bt.travIn(iter);
    printf("\n");
    // bt.travPost(iter);
    // printf("\n");
    bt.travLevel(iter);
    printf("\n");
    return 0;
}