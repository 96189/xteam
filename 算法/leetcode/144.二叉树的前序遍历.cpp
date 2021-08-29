/*
 * @lc app=leetcode.cn id=144 lang=cpp
 *
 * [144] 二叉树的前序遍历
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
    // void preorderTraversalHelper(TreeNode* root, std::vector<int> &ans) {
    //     if (!root) {
    //         return;
    //     }
    //     ans.push_back(root->val);
    //     preorderTraversalHelper(root->left, ans);
    //     preorderTraversalHelper(root->right, ans);
    // }
    // vector<int> preorderTraversal(TreeNode* root) {
    //     std::vector<int> ans;
    //     preorderTraversalHelper(root, ans);
    //     return ans;
    // }
    vector<int> preorderTraversal(TreeNode* root) {
        std::vector<int> ans;
        std::stack<TreeNode*> s;
        if (root) {
            s.push(root);
        }
        while (!s.empty()) {
            TreeNode* pCur = s.top();
            ans.push_back(pCur->val);
            s.pop();
            if (pCur->right) {
                s.push(pCur->right);
            }
            if (pCur->left) {
                s.push(pCur->left);
            }
        }
        return ans;
    }
};
// @lc code=end

