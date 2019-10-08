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
    // int arr[] = { -2, -1, -3, -5, -9, -8, 1, 3, 5, 7, 9, 2, 4, 6, 8, 0 };
    // int arr[] = { 1, 3, 5, 7, 9, 2, 4, 6, 8, 0 };
    // int arr[] = { 5, 6, 4, 7, 3, 8, 2, 9, 0, 1};
    // int len = sizeof(arr) / sizeof(arr[0]);
    // std::vector<ElemType> data;
    // data.assign(arr, arr + len);
    
    // TreeNode* root = NULL;
    // BuildTree(&root, data);
    // assert(NULL != root);

    // printf("root:%d\n", root->obj_);

    // FirstTraver(root);
    // printf("\n");
    // MidTraver(root);
    // printf("\n");
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
    // while (root)
    // {
    //     int index = rand() % len;
    //     printf("delete %d\n", arr[index]);
    //     bool flag = Delete(root, arr[index]);
    //     if (flag)
    //     {
    //         MidTraver(root);
    //         printf("\n");
    //     }
    //     if (!root->left_ && !root->right_)
    //     {
    //         break;
    //     }
    // }
    // MidTraverIteration(root);
    // FirstTraver(root);
    // printf("\n");
    // FirstTraverIteration(root);
    
    // LastTraver(root);
    // printf("\n");
    // LastTraverIteration(root);

    // BFIteration(root);
    // DFIteration(root);
    int first[] = { 5, 3, 2, 4, 8, 6, 9 };
    int flen = sizeof(first) / sizeof(first[0]);
    int middle[] = { 2, 3, 4, 5, 6, 8, 9 };
    int mlen = sizeof(middle) / sizeof(middle[0]);
    int last[] = { 2, 4, 3, 6, 9, 8, 5 };
    int llen = sizeof(last) / sizeof(last[0]);
    // TreeNode* pRoot = FMOrderBuildTree(first, 0, flen-1, middle, 0, mlen-1);
    TreeNode* pRoot = MLOrderBuikdTree(middle, 0, mlen-1, last, 0, llen-1);
    // assert(pRoot->obj_ == 5);
    FirstTraver(pRoot);
    printf("\n");
    MidTraver(pRoot);
    printf("\n");
    return 0;
}