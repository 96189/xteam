/*
 * @lc app=leetcode.cn id=145 lang=cpp
 *
 * [145] 二叉树的后序遍历
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
    // void postorderTraversalHelper(TreeNode *root, std::vector<int> &ans) {
    //     if (!root) {
    //         return;
    //     }
    //     postorderTraversalHelper(root->left, ans);
    //     postorderTraversalHelper(root->right, ans);
    //     ans.push_back(root->val);
    // }
    // vector<int> postorderTraversal(TreeNode* root) {
    //     std::vector<int> ans;
    //     postorderTraversalHelper(root, ans);
    //     return ans;
    // }
    vector<int> postorderTraversal(TreeNode* root) {
        std::vector<int> ans;
        std::stack<TreeNode*> s;
        TreeNode *pCur = root;
        TreeNode *prev = nullptr;
        while (!s.empty() || pCur) {
            while (pCur) {
                s.push(pCur);
                pCur = pCur->left;
            }
            pCur = s.top();
            s.pop();
            if (pCur->right == nullptr || pCur->right == prev) {
                ans.push_back(pCur->val);
                prev = pCur;
                pCur = nullptr;
            } else {
                s.push(pCur);
                pCur = pCur->right;
            }
        }
        return ans;
    }
};
// @lc code=end

