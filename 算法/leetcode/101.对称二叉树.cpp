/*
 * @lc app=leetcode.cn id=101 lang=cpp
 *
 * [101] 对称二叉树
 */

// @lc code=start
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    bool isSymmetricHelp(TreeNode* left, TreeNode* right) {
        if (!left && !right) {
            return true;
        } else if (!left || !right) {
            return false;
        }
        return left->val == right->val && isSymmetricHelp(left->left, right->right) && isSymmetricHelp(left->right, right->left);

    }

    // 递归版
    bool isSymmetric(TreeNode* root) {
        return root && isSymmetricHelp(root->left, root->right);
    }

    // // 迭代版
    // bool isSymmetric(TreeNode* root) {
    //     if (nullptr == root) {
    //         return false;
    //     }
    //     std::queue<TreeNode *> q;
    //     q.push(root->left);
    //     q.push(root->right);

    //     while (!q.empty()) {
    //         TreeNode *ltree = q.front();
    //         q.pop();
    //         TreeNode *rtree = q.front();
    //         q.pop();
    //         // 跳过空叶子节点
    //         if (!ltree && !rtree) {
    //             continue;
    //         }
    //         if (!ltree || !rtree) {
    //             return false;
    //         }
    //         if (ltree->val != rtree->val) {
    //             return false;
    //         }
    //         q.push(ltree->left);
    //         q.push(rtree->right);
    //         q.push(ltree->right);
    //         q.push(rtree->left);
    //     }
    //     return true;
    // }
};
// @lc code=end

