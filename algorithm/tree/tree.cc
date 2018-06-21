#include <vector>
#include "tree.h"
#include <stdlib.h>

void BuildTree(TreeNode** root, std::vector<ElemType>& vec)
{   
    for (std::vector<ElemType>::iterator it = vec.begin(); it != vec.end(); ++it)
    {
        *root = Insert(*root, *it);
    }
}

int main(int argc, char* argv[])
{
    int arr[] = { -2, -1, -3, -5, -9, -8, 1, 3, 5, 7, 9, 2, 4, 6, 8, 0 };
    int len = sizeof(arr) / sizeof(arr[0]);
    std::vector<ElemType> data;
    data.assign(arr, arr + len);
    
    TreeNode* root = NULL;
    BuildTree(&root, data);
    assert(NULL != root);

    // FirstTraver(root);
    // printf("\n");
    MidTraver(root);
    printf("\n");
    // LastTraver(root);
    // printf("\n");

    // int dep = Depth(root);
    // printf("%d\n", dep);

    // TreeNode* parent = NULL;
    // TreeNode* p = Search(root, 6, &parent);
    // assert(6 == p->obj_);
    // assert(7 == parent->obj_);

    // parent = NULL;
    // TreeNode* p2 = Search(root, 13, &parent);
    // assert(p2 == NULL);

    // parent = NULL;
    // TreeNode* pMax = BSTMaxTreeNode(root, &parent);
    // assert(pMax->obj_ == 9);
    // assert(parent->obj_ == 7);

    // 9 8 0 1
    while (root)
    {
        int index = rand() % len;
        printf("delete %d\n", arr[index]);
        bool flag = Delete(root, arr[index]);
        if (flag)
        {
            MidTraver(root);
            printf("\n");
        }
        if (!root->left_ && !root->right_)
        {
            break;
        }
    }
    return 0;
}