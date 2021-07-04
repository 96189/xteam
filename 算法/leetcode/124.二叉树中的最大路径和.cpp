/*
 * @lc app=leetcode.cn id=124 lang=cpp
 *
 * [124] 二叉树中的最大路径和
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
    int maxPathSumHelper(TreeNode* root, int &res) {
        if (nullptr == root) {
            return 0;
        }
        int left = std::max(maxPathSumHelper(root->left, res), 0);
        int right = std::max(maxPathSumHelper(root->right, res), 0);
        res = std::max(res, left + right + root->val);
        return std::max(left, right) + root->val;
    }
    int maxPathSum(TreeNode* root) {
        int res = INT_MIN;
        maxPathSumHelper(root, res);
        return res;
    }
};
// @lc code=end

