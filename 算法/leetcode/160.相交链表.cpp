/*
 * @lc app=leetcode.cn id=160 lang=cpp
 *
 * [160] 相交链表
 */

// @lc code=start
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
    int list_len(ListNode *head) {
        int len = 0;
        while (head) {
            ++len;
            head = head->next;
        }
        return len;
    }
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        int lenA = list_len(headA);
        int lenB = list_len(headB);
        ListNode *list1 = headA;
        ListNode *list2 = headB;
        if (lenA < lenB) {
            list1 = headB;
            list2 = headA;
        }
        int diff_len = abs(lenA - lenB);
        while (diff_len-- > 0 && list1) {
            list1 = list1->next;
        }
        while (list1 && list2) {
            if (list1 == list2) {
                return list1;
            }
            list1 = list1->next;
            list2 = list2->next;
        }
        return nullptr;
    }
    // // 破坏链表结构的解法
    // ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
    //     ListNode *pPrev = headA;
    //     ListNode *pCur = headA;
    //     while (pCur) {
    //         pPrev = pCur;
    //         pCur = pCur->next;
    //         pPrev->next = nullptr;
    //     }

    //     pPrev = headB;
    //     pCur = headB;
    //     while (pCur) {
    //         pPrev = pCur;
    //         pCur = pCur->next;
    //     }
    //     return pPrev;
    // }
};
// @lc code=end

