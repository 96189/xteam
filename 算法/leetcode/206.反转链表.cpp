/*
 * @lc app=leetcode.cn id=206 lang=cpp
 *
 * [206] 反转链表
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
    // 1        2       3       4       5
// pPrev pCur    pNext 
//       pPrev   pCur    pNext
//               pPrev   pCur    pNext 
//                       pPrev   pCur    pNext 
//                               pPrev   pCur    pNext 
    ListNode* reverseList(ListNode* head) {
        ListNode *pPrev = nullptr;
        ListNode *pCur = head;
        while (pCur) {
            ListNode *pNext = pCur->next;
            pCur->next = pPrev;
            pPrev = pCur;
            pCur = pNext;
        }
        // head->next = nullptr;
        return pPrev;
    }
    // ListNode* reverseList(ListNode* head) {
    //     ListNode *pCur = head;
    //     if (!pCur || !pCur->next) {
    //         return pCur;
    //     }
    //     ListNode *newHead = reverseList(pCur->next);
    //     pCur->next->next = pCur;
    //     pCur->next = nullptr;
    //     return newHead;
    // }
};
// @lc code=end

