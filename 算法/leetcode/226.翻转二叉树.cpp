/*
 * @lc app=leetcode.cn id=226 lang=cpp
 *
 * [226] 翻转二叉树
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
    // TreeNode* invertTree(TreeNode* root) {
    //     if (!root) {
    //         return root;
    //     }
    //     TreeNode *l = root->left;
    //     root->left = root->right;
    //     root->right = l;
    //     invertTree(root->left);
    //     invertTree(root->right);
    //     return root;
    // }

    TreeNode* invertTree(TreeNode* root) {
        std::queue<TreeNode*> q;
        if (root) {
            q.push(root);
        }
        while (!q.empty()) {
            TreeNode *pCur = q.front();
            q.pop();
            TreeNode *l = pCur->left;
            pCur->left = pCur->right;
            pCur->right = l;
            if (pCur->left) {
                q.push(pCur->left);
            }
            if (pCur->right) {
                q.push(pCur->right);
            }
        }
        return root;
    }
};
// @lc code=end

