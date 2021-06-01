/*
 * @lc app=leetcode.cn id=24 lang=cpp
 *
 * [24] 两两交换链表中的节点
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
    ListNode* swapPairs(ListNode* head) {
        ListNode *dummy = new ListNode();
        dummy->next = head;
        ListNode *tail = dummy;
        ListNode *pCur = dummy->next;
        while (pCur) {
            ListNode *pNext = pCur->next;
            if (!pNext) {
                break;
            }
            pCur->next = pNext->next;
            pNext->next = pCur;
            tail->next = pNext;
            
            tail = pCur;
            pCur = pCur->next;
        }
        return dummy->next;
    }
};
// @lc code=end

