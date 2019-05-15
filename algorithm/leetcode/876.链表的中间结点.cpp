/*
 * @lc app=leetcode.cn id=876 lang=cpp
 *
 * [876] 链表的中间结点
 */
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* middleNode(ListNode* head) {
        ListNode *slow = head->next;
        if (!slow)
            return head;
        ListNode *fast = NULL;
        if (slow->next)
            fast = slow->next->next;
        if (!slow->next || !fast)
            return slow;

        while (fast)
        {
            slow = slow->next;
            if (fast->next)
                fast = fast->next->next;
            else
                fast = fast->next;
        }
        return slow;
    }
};

