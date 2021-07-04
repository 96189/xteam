/*
 * @lc app=leetcode.cn id=94 lang=cpp
 *
 * [94] 二叉树的中序遍历
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
    // 递归版本
    // void inorderTraversalHelper(TreeNode* root, std::vector<int> &ans) {
    //     if (nullptr == root) {
    //         return;
    //     }
    //     inorderTraversalHelper(root->left, ans);
    //     ans.push_back(root->val);
    //     inorderTraversalHelper(root->right, ans);
    // }
    // vector<int> inorderTraversal(TreeNode* root) {
    //     std::vector<int> ans;
    //     inorderTraversalHelper(root, ans);
    //     return ans;
    // }

    // 非递归版本
    vector<int> inorderTraversal(TreeNode* root) {
        std::vector<int> ans;
        std::stack<TreeNode*> s;
        TreeNode *pCur = root;
        while (!s.empty() || pCur) {
            if (pCur) {
                s.push(pCur);
                pCur = pCur->left;
            } else {
                pCur = s.top();
                s.pop();
                ans.push_back(pCur->val);
                pCur = pCur->right;
            }
        }
        return ans;
    }
};
// @lc code=end

