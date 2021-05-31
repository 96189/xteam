/*
 * @lc app=leetcode.cn id=19 lang=cpp
 *
 * [19] 删除链表的倒数第 N 个结点
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
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode *pPrev = head;
        ListNode *pCur = head;
        ListNode *pFast = head;

        // 快指针走n步
        while (pFast && n-- > 0) {
            pFast = pFast->next;
        }
        if (!pFast) {
            head = pCur->next;
            delete pCur;
            return head;
        }

        while (pFast) {
            pPrev = pCur;
            pCur = pCur->next;
            pFast = pFast->next;
        }
        if (pCur == head) {
            head = head->next;
            delete pCur;
        } else {
            pPrev->next = pCur->next;
            delete pCur;
        }
        return head;
    }
};
// @lc code=end

