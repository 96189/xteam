/*
 * @lc app=leetcode.cn id=61 lang=cpp
 *
 * [61] 旋转链表
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
    ListNode* rotateRight(ListNode* head, int k) {
        if (!head || !head->next) {
            return head;
        }
        int count = 0;
        ListNode *pCur = head;
        while (pCur) {
            ++count;
            pCur = pCur->next;
        }
        if (k >= count) {
            k =  k % count;
        }
        if (0 == k) {
            return head;
        }

        ListNode *dummy = new ListNode();
        dummy->next = head;
        ListNode *pFastK = head;
        while (k > 0) {
            pFastK = pFastK->next;
            --k;
        }
        ListNode *pSlow = head;
        while (pFastK && pFastK->next) {
            pSlow = pSlow->next;
            pFastK = pFastK->next;
        }
        pFastK->next = dummy->next;
        dummy->next = pSlow->next;
        pSlow->next = nullptr;
        
        return dummy->next;
    }
};
// @lc code=end

