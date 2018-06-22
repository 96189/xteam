#include "avl_tree.h"
#include <vector>
#include <stdlib.h>

void BuildTree(TreeNode **root, std::vector<ElemType> &vec)
{
    for (std::vector<ElemType>::iterator it = vec.begin(); it != vec.end(); ++it)
    {
        *root = Insert(*root, *it);
    }
}

void PrinTreeDetail(TreeNode* root)
{
    std::list<TreeNode *> l;
    MidTraverSave(root, l);
    for (std::list<TreeNode *>::iterator it = l.begin(); it != l.end(); it++)
    {
        printf("addr:%p ", *it);
        printf("value:%d ", (*it)->obj_);
        printf("height_:%d ", (*it)->height_);
        printf("parent:%p ", (*it)->parent_);
        printf("left:%p ", (*it)->left_);
        printf("right:%p ", (*it)->right_);
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    // int arr[] = {-2, -1, -3, -5, -9, -8, 1, 3, 5, 7, 9, 2, 4, 6, 8, 0 /**/};
    // int arr[] = { 0, 1, 2, 3/*, 4, 5, 6, 7, 8, 9*/ };
    int arr[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
    int len = sizeof(arr) / sizeof(arr[0]);
    std::vector<ElemType> data;
    data.assign(arr, arr + len);

    TreeNode *root = NULL;
    BuildTree(&root, data);
    assert(NULL != root);

    PrinTreeDetail(root);

    MidTraver(root);
    printf("\n");
    
    // TreeNode* pNode = Search(root, 8);
    // assert(pNode->obj_ == 8);
    // TreeNode* pNode1 = Search(root, 18);
    // assert(pNode1 == NULL);
    // assert(Delete(root, 8) == true);
    // assert(Delete(root, 19) == false);

    while (root)
    {
        int index = rand() % len;
        printf("delete %d\n", arr[index]);
        bool flag = Delete(root, arr[index]);
        if (flag)
        {
            PrinTreeDetail(root);
            MidTraverCheckBalance(root);
            printf("\n");
        }
        if (!root->left_ && !root->right_)
        {
            break;
        }
    }
    return 0;
}