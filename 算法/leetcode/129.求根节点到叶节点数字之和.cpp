/*
 * @lc app=leetcode.cn id=129 lang=cpp
 *
 * [129] 求根节点到叶节点数字之和
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
    int sumNumbersHelper(TreeNode* root, int sum) {
        if (nullptr == root) {
            return 0;
        }
        sum = sum * 10 + root->val;
        if (nullptr == root->left && nullptr == root->right) {
            return sum;
        }
        return sumNumbersHelper(root->left, sum) + sumNumbersHelper(root->right, sum);
    }
    int sumNumbers(TreeNode* root) {
        return sumNumbersHelper(root, 0);
    }
};
// @lc code=end

