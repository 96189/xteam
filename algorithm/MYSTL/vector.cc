#include "vector.h"

#include <stdio.h>

// [start, end)
template<typename T>
void checkVector(MYSTL::Vector<T>& v, const T* arr, int start, int end)
{
    for (int i = start; i < end; ++i)
    {
        assert(v[i-start] == arr[i]);
    }
}
template<typename T>
void checkVector(MYSTL::Vector<T>& v, const MYSTL::Vector<T>& vv, int start, int end)
{
    for (int i = start; i < end; ++i)
    {
        assert(v[i-start] == vv[i]);
    }
}
void PrintInt(int& val)
{
    printf("%d ", val);
}

void TestSort()
{
    printf("\ntest sort start...\n");
    int arr[] = { 3, 22, 55, 11, 1, 66, 71, 89, 9, 10, 10 };
    int len = sizeof(arr)/ sizeof(arr[0]);
    MYSTL::Vector<int> v(arr, 0, len);
    v.sort();
    v.traverse(PrintInt);    
    printf("\ntest sort end...\n");
}

void TestConstructor()
{
    const int n = 11;
    MYSTL::Vector<int> v;
    assert(v.size() == 0 && v.empty());
    // 包含0个逆序对
    for (int i = 0; i < n; ++i)
    {
        v.insert(i,i);
        assert(v[i] == i);
    }

    int arr[n] = { 3, 22, 55, 11, 1, 66, 71, 89, 9, 10, 10 };
    int start = 3;
    int end = 9;
    // 11, 1, 66, 71, 89, 9 => 包含2个逆序对
    MYSTL::Vector<int> vv(arr, start, end);
    checkVector(vv, arr, start, end);

    start = 1;
    end = 4;
    MYSTL::Vector<int> vvv(vv, start, end);
    checkVector(vvv, vv, start, end);

    MYSTL::Vector<int> vvvv(vvv);
    checkVector(vvvv, vvv, 0, vvv.size());
}

void TestWriteInterface(MYSTL::Vector<int> & vv)
{
    // 11, 1, 66, 71, 89, 9
    int idx = vv.find(3);
    assert(idx == -1);
    idx = vv.find(66);
    assert(idx == 2);

    int count = vv.remove(2, 4);
    assert(count == (4-2) && vv.size() == 4);

    // 11, 1, 89, 9
    int val = vv.remove(2);
    assert(vv.size() == 3 && val == 89);

    // 11, 1, 9
    vv.insert(11);
    vv.insert(1);
    vv.insert(9);
    assert(vv.size() == 6);

    count = vv.deduplicate();
    assert(count == 3 && vv[0] == 11 && vv[1] == 1 && vv[2] == 9);

    vv.insert(11);
    vv.sort();
    vv.traverse(PrintInt);
}

void TestReadInterface(MYSTL::Vector<int> & vv)
{
    int pair = vv.disordered();
    assert(pair == 0);
    // 1 9 11 11
    assert(vv.search(11) == 3);
    assert(vv.search(8) == 0);
    printf("\n");
    vv.unsort();
    vv.traverse(PrintInt);
}

int main(int argc, char* argv[])
{
    TestConstructor();
    int arr[] = { 11, 1, 66, 71, 89, 9 };
    MYSTL::Vector<int> vv(arr, 0, sizeof(arr)/sizeof(arr[0]));
    TestWriteInterface(vv);
    TestReadInterface(vv);
    TestSort();
    return 0;
}
