/*
 * @lc app=leetcode.cn id=234 lang=cpp
 *
 * [234] 回文链表
 */

// @lc code=start
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        ListNode *slow = head;
        ListNode *fast = head;
        while (nullptr != fast->next && nullptr != fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        stack<int> half;
        while (nullptr != slow) {
            half.push(slow->val);
            slow = slow->next;
        }
        while (!half.empty()) {
            int val = head->val;
            int h_val = half.top();
            if (val != h_val) {
                return false;
            }
            head = head->next;
            half.pop();
        }
        return true;
    }
};
// @lc code=end

