#include "list.h"

using namespace MYSTL;

void PrintInt(int& val)
{
    printf("%d ", val);
}

void TestMiddleNode()
{
    MYSTL::List<int> l;
    l.insertAsLast(1);
    ListNodePosi(int) pCur = l.MiddleNode();
    assert(pCur->getData() == 1);

    l.insertAsLast(2);
    pCur = l.MiddleNode();
    assert(pCur->getData() == 1);

    l.insertAsLast(3);
    pCur = l.MiddleNode();
    assert(pCur->getData() == 2);

    l.insertAsLast(4);
    pCur = l.MiddleNode();
    assert(pCur->getData() == 2);

    l.insertAsLast(5);
    l.insertAsLast(6);
    pCur = l.MiddleNode();
    assert(pCur->getData() == 3);
}

void TestIsLoop()
{
    MYSTL::List<int> l;
    assert(l.IsLoop() == false);
    l.insertAsLast(1);
    assert(l.IsLoop() == false);
    l.MakeLoop(1);
    assert(l.IsLoop() == true);
    l.insertAsLast(2);
    l.insertAsLast(3);
    l.insertAsLast(4);
    l.insertAsLast(5);
    l.MakeLoop(3);
    assert(l.IsLoop() == true);
    printf("TestIsLoop end\n");
    // 成环导致析构时 对同一个地址多次释放 core
    getchar();
}

int main(int argc, char* argv[])
{
    // MYSTL::List<int> l;
    // assert(l.size() == 0);
    // assert(sizeof(l) == 24 || sizeof(l) == 12);
    // assert(l.size() == 0 && l.empty());
    // // 头插
    // l.insertAsFirst(5);
    // l.insertAsFirst(3);
    // l.insertAsFirst(1);
    // l.traverse(PrintInt);
    // printf("\n");
    // assert(l.first()->getData() == 1 && l.last()->getData() == 5);
    // assert(l.size() == 3);
    // // 尾插
    // l.insertAsLast(9);
    // l.insertAsLast(2);
    // l.insertAsLast(4);
    // l.insertAsLast(6);
    // l.traverse(PrintInt);
    // printf("\n");
    // assert(l.first()->getData() == 1 && l.last()->getData() == 6);
    // assert(l.size() == 7);
    
    // // 列表查找 指定位置前插入
    // ListNodePosi(int) pCur = l.find(5);
    // assert(pCur->getData() == 5);
    // l.insertAsSucc(pCur, 5);

    // // 列表查找 指定位置之后插入
    // pCur = l.find(2, 3, l.last());
    // assert(pCur->getData() == 2);
    // l.insertAsPred(pCur, 2);

    // l.traverse(PrintInt);
    // printf("\n");

    // // 1 3 5 5 9 2 2 4 6
    // int count = l.disordered();
    // assert(count == 1);
    
    // pCur = l.find(9);
    // int val = l.remove(pCur);
    // // assert(l.valid(pCur) == false);
    // l.traverse(PrintInt);
    // printf("\n");

    // l.reverse();
    // l.traverse(PrintInt);
    // printf("\n");

    // l.reverse();
    // l.traverse(PrintInt);
    // printf("\n");

    // val = l.selectMax();
    // assert(val == 6);

    // printf("copy construct\n");
    // List<int> ll(l);
    // List<int> lll(l, 0, l.size());
    // List<int> llll(l.first(), l.size());
    // ll.traverse(PrintInt);
    // printf("\n");
    // lll.traverse(PrintInt);
    // printf("\n");
    // llll.traverse(PrintInt);
    // printf("\n");

    // // sort
    // printf("sort\n");
    // l.sort();
    // l.traverse(PrintInt);
    // printf("\n");

    // // 1 3 5 5 2 2 4 6
    // count = l.deduplicate();
    // assert(count == 2);
    // l.traverse(PrintInt);
    // printf("\n");

    // TestMiddleNode();
    TestIsLoop();
    return 0;
}