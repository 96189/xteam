/*
 * @lc app=leetcode.cn id=92 lang=cpp
 *
 * [92] 反转链表 II
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
    // 改指针不改值
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        // 固定头
        ListNode *dummy = new ListNode();
        dummy->next = head;

        ListNode *pFront = nullptr;
        ListNode *pRear = nullptr;

        ListNode *newHead = nullptr;
        ListNode *pCur = dummy;
        for (int i = 1; i < left; ++i) {
            pCur = pCur->next;
        }
        // 头尾
        pFront = pCur;
        pRear = pCur->next;

        for (int i = left; i <= right; ++i) {
            // 断链节点
            pCur = pFront->next;
            // 下一个断链节点
            pFront->next = pCur->next;
            // 之前的链向当前断链节点之后弥补
            pCur->next = newHead;
            // 当前节点成为新链头
            newHead = pCur;
        }

        // 最后一个节点
        pCur = pFront->next;
        // 最后一个节点链接
        pRear->next = pCur;
        // 重新挂接第一个断开的地方
        pFront->next = newHead;
        return dummy->next;
    }
    // 改值不改指针
    // ListNode* reverseBetween(ListNode* head, int left, int right) {
    //     std::vector<int> v;
    //     v.resize(right - left + 1);
    //     int count = 0;
    //     ListNode *pCur = head;
    //     while (pCur) {
    //         ++count;
    //         if (count >= left && count <= right) {
    //             v.push_back(pCur->val);
    //         }
    //         pCur = pCur->next;
    //     }

    //     pCur = head;
    //     count = 0;
    //     int i = v.size();
    //     while (pCur) {
    //         ++count;
    //         if (count >= left && count <= right) {
    //             pCur->val = v[--i];
    //         }
    //         pCur = pCur->next;
    //     }
    //     return head;
    // }
};
// @lc code=end

