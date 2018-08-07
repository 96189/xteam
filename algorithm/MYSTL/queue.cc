#include "queue.h"

void PrintInt(int& val)
{
    printf("%d ", val);
}

int main(int argc, char* argv[])
{
    MYSTL::Queue<int> q;
    for (int i = 0; i < 10; i++)
    {
       if (i % 2 != 0) 
       {
           q.enqueue(i);
       }
    }
    q.traverse(PrintInt);
    printf("\n");
    assert(q.size() == 5);
    assert(q.front() == 1);
    assert(q.rear() == 9);
    while (!q.empty())
    {
        printf("dequeue:%d\n", q.dequeue());
    }
    printf("\n");
    assert(q.size() == 0);
    return 0;
}