/*
 * @lc app=leetcode.cn id=102 lang=cpp
 *
 * [102] 二叉树的层序遍历
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
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> vv;
        std::queue<TreeNode*> q;
        if (root) {
            q.push(root);
        }
        while (!q.empty()) {
            vector<int> v;
            size_t len = q.size();
            for (size_t i = 0; i < len; ++i) {
                TreeNode *pCur = q.front();
                v.push_back(pCur->val);
                q.pop();
                if (pCur->left) {
                    q.push(pCur->left);
                }
                if (pCur->right) {
                    q.push(pCur->right);
                }
            }
            vv.push_back(v);
        }
        return vv;
    }
};
// @lc code=end

