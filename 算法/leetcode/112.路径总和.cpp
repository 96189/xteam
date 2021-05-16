/*
 * @lc app=leetcode.cn id=112 lang=cpp
 *
 * [112] 路径总和
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
    bool hasPathSumHelper(TreeNode* node, int sum, const int targetSum) {
        if (nullptr == node) {
            return false;
        }
        sum += node->val;
        if (sum == targetSum && !node->left && !node->right) {
            return true;
        }
        return hasPathSumHelper(node->left, sum, targetSum) || hasPathSumHelper(node->right, sum, targetSum);
    }

    // // 递归版
    // bool hasPathSum(TreeNode* root, int targetSum) {
    //     int sum = 0;
    //     return root && hasPathSumHelper(root, sum, targetSum);
    // }

    // 迭代版
    bool hasPathSum(TreeNode* root, int targetSum) {
        if (nullptr == root) {
            return false;
        }
        std::stack<TreeNode*> nodes;
        TreeNode* curNode = root;
        TreeNode* preNode = NULL;
        while (curNode || !nodes.empty()) {
            while (curNode) {
                nodes.push(curNode);
                targetSum -= curNode->val;
                curNode = curNode->left;
            }
            curNode = nodes.top();
            if (!curNode->left && !curNode->right && 0 == targetSum) {
                return true;
            }

            // 回溯
            if (curNode->right && preNode != curNode->right) {
                curNode = curNode->right;
            } else {
                nodes.pop();
                targetSum += curNode->val;
                preNode = curNode;
                curNode = nullptr;
            }
        }
        return false;
    }
};
// @lc code=end

