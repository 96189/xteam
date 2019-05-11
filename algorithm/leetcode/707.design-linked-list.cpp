/*
 * @lc app=leetcode.cn id=707 lang=cpp
 *
 * [707] Design Linked List
 */
class MyLinkedList {
public:
    /** Initialize your data structure here. */
    MyLinkedList() {
        headNode_ = tailNode_ = new ListNode(0);
        len_ = 0;
    }
    
    /** Get the value of the index-th node in the linked list. If the index is invalid, return -1. */
    int get(int index) {
        if (index < 0 || index > len_)
            return -1;
        ListNode* pCurNode = headNode_;
        while (index-- > 0)
        {
            pCurNode = pCurNode->next_;
        }
        return pCurNode->val_;
    }
    
    /** Add a node of value val before the first element of the linked list. After the insertion, the new node will be the first node of the linked list. */
    void addAtHead(int val) {
        addAtIndex(0, val);
    }
    
    /** Append a node of value val to the last element of the linked list. */
    void addAtTail(int val) {
        addAtIndex(len_, val);
    }
    
    /** Add a node of value val before the index-th node in the linked list. If index equals to the length of linked list, the node will be appended to the end of linked list. If index is greater than the length, the node will not be inserted. */
    void addAtIndex(int index, int val) {
        if (index > len_)
            return;
        ListNode* newNode = new ListNode(val);
        if (index == len_)
        {
            if (headNode_ == tailNode_)
            {
                headNode_->next_ = newNode;
                tailNode_ = newNode;
            }
            else 
            {
                tailNode_->next_ = newNode;
                tailNode_ = newNode;
            }
        }
        // index < len
        else 
        {
            ListNode* prevNode = headNode_;
            while (--index > 0)
            {
                prevNode = prevNode->next_;
            }

            ListNode* nextNode = prevNode->next_;
            newNode->next_ = nextNode;
            prevNode->next_ = newNode; 
        }
        ++len_;
    }
    
    /** Delete the index-th node in the linked list, if the index is valid. */
    void deleteAtIndex(int index) {
        if (index < 0 || index > len_)
            return;
        ListNode* prevNode = headNode_;
        while (--index > 0)
        {
            prevNode = prevNode->next_;
        }
        ListNode* delNode = prevNode->next_;
        prevNode->next_ = delNode->next_;
        delete delNode;
        delNode = NULL;
        --len_;
    }
private:
    struct ListNode
    {
        int val_;
        struct ListNode *next_;
        ListNode(int val)
        {
            val_ = val;
            next_ = NULL;
        }
    };
    ListNode *headNode_;
    ListNode *tailNode_;
    int len_;
};

/**
 * Your MyLinkedList object will be instantiated and called as such:
 * MyLinkedList* obj = new MyLinkedList();
 * int param_1 = obj->get(index);
 * obj->addAtHead(val);
 * obj->addAtTail(val);
 * obj->addAtIndex(index,val);
 * obj->deleteAtIndex(index);
 */

