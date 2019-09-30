#include "rb_tree.h"
#include <vector>

void BuildTree(RBTNode **root, std::vector<ElemType> &vec)
{
    for (std::vector<ElemType>::iterator it = vec.begin(); it != vec.end(); ++it)
    {
        Insert(root, *it);
    }
}

int main(int argc, char *argv[])
{
    char *color[] = {"RED", "BLACK"};
    int arr[] = {1, 3, 5, 7, 9, 2, 4, 6, 8, 0 /**/};
    int len = sizeof(arr) / sizeof(arr[0]);
    std::vector<ElemType> data;
    data.assign(arr, arr + len);

    RBTNode *root = NULL;
    BuildTree(&root, data);
    assert(NULL != root);
    std::list<RBTNode *> l;
    MidTraverSave(root, l);
    for (std::list<RBTNode *>::iterator it = l.begin(); it != l.end(); it++)
    {
        printf("addr:%p ", *it);
        printf("value:%d ", (*it)->obj_);
        printf("color:%s ", color[(*it)->color_]);
        printf("parent:%p ", (*it)->parent_);
        printf("left:%p ", (*it)->left_);
        printf("right:%p ", (*it)->right_);
        printf("\n");
    }
    MidTraver(root);
    printf("\n");
    return 0;
}