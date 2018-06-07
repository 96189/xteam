#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
class Node
{
public:
	int val_;
	Node* next_;

	Node()
	{

	}
	Node(int val) : val_(val)
	{
		next_ = NULL;
	}
};
// 单链表快慢指针判断是否存在环
bool IsCircle(Node* head, Node** pMeetPoint)
{
	bool bcircle = false;
	Node* fast = head->next_;
	assert(fast);
	Node* slow = head->next_;
	assert(slow);
	while (fast && fast->next_ != NULL)
	{
		fast = fast->next_->next_;
		slow = slow->next_;
		if (fast == slow)
		{
			break;
		}
	}
	if (fast && fast->next_ != NULL)
	{
		*pMeetPoint = fast;
		bcircle = true;
	}
	return bcircle;
}
// 可证明
static Node* JoinPoint(Node* head, Node** end)
{
	Node* start = head->next_;
	while (start != *end)
	{
		start = start->next_;
		(*end) = (*end)->next_;
	}
	return start;
}
// 查找单链表环的进入点
Node* FindJoinCircleStart(Node* head)
{
	Node* joinPoint = NULL;
	Node* pMeetPoint = NULL;
	bool bCircle = IsCircle(head, &pMeetPoint);
	if (bCircle)
	{
		joinPoint = JoinPoint(head, &pMeetPoint);
	}
	return joinPoint;
}

Node* CreateList(int n)
{
	Node* head = new Node(n);
	for (int i = 1; i < n; ++i)
	{
		Node* pCur = new Node(i);
		pCur->next_ = head->next_;
		head->next_ = pCur;
	}
	return head;
}
void IteratorList(Node* head)
{
	Node* pCur = head;
	while (pCur != NULL)
	{
		printf("%d\n", pCur->val_);
		pCur = pCur->next_;
	}
}
void MakeCircle(Node* head)
{
	Node* point = NULL;
	Node* pCur = head->next_;
	for (int i = 0; i < 3; ++i)
	{
		point = pCur;
		pCur = point->next_;
	}
	while (pCur->next_ != NULL)
	{
		pCur = pCur->next_;
	}
	pCur->next_ = point;

}	
Node* Reverse(Node* pNode)
{
	Node* prev = pNode;
	Node* pCur = prev->next_;

	// 第一个节点作最后一个节点
	prev->next_ = NULL;
	
	Node* pNext = NULL;
	while (pCur != NULL)
	{
		// 保存下一个要处理的节点
		pNext = pCur->next_;
		// 改变当前节点的指向
		pCur->next_ = prev;

		// 3个节点步进
		prev = pCur;
		pCur = pNext;
		// pNext可能为NULL
		if (pNext)
		{
			pNext = pNext->next_;
		}
	}

	return prev;
}
int main(int argc, char* argv[])
{
	Node* h = CreateList(10);
	IteratorList(h);
	// MakeCircle(h);
	// Node* meetPoint;
	// printf("IsCircle %d\n", IsCircle(h, &meetPoint));
	// Node* joinPoint = FindJoinCircleStart(h);
	// printf("joinPoint:%d\n", joinPoint->val_);
	Node* newh = Reverse(h);
	IteratorList(newh);
	return 0;
}