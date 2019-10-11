
#include <vector>
#include <numeric>
using namespace std;

// n个正整数 分成连续m份 要求最大的那一份的数字之和尽量小(划分有多种方案,每种方案都会有和最大的值,选取方案中和最小的那一个)
// 转化:最大的那一份数字之和不超过给定的数字d,是否能划分成连续的m份
// 从左到右尽量将更多的元素划分进一个组里,这样可以得到至少要分成多少份,假设为cnt
// 若cnt<=m 那就是能划分的 由于少的那些组可以通过拆分cnt组
// 分组
// 
// 判断数组A 是否能分成m份 满足每一份之和不超过d
bool check(vector<int> &A, long long d, int m)
{
    int n = A.size();
    long long sum = 0;
    int cnt = 1;    // 每一份数字之和不超过d的情况下 至少要分成的份数
    for (int i = 0; i < n; ++i)
    {
        if (A[i] > d)   // 单个元素大于d
            return false;
        sum += A[i];
        if (sum > d)
        {
            sum = A[i];
            ++cnt;
        }
    }
    return cnt <= m;
}
// 将所给的数组分成连续的m份 似的数字之和最大的那一份数字之和最小
// 返回值 最优方案中 数字最大的那一份的数字之和
long long getAnswer(vector<int>& A, int m)
{
    int n = A.size();
    long long l = 1;    // 下界
    long long r = 0;    // 上界
    accumulate(A.begin(), A.end(), r);

    while (l <= r)
    {
        long long mid = (l+r) >> 1;
        if (check(A, mid, m))
            r = mid - 1;
        else 
            r = mid + 1;
    }
    return r+1;
}

// 二分查找
// 正整数序列中查找第一个大于等于key的元素
int binsearch(vector<int> &A, int key)
{
    int n = A.size();
    int l = -1;
    int r = n;
    int mid;
    while (l+1 < n)
    {
        mid = (l+r) >> 1;
        if (A[mid] < key)
        {
            l = mid;
        }
        else 
        {
            r = mid;
        }
    }
    if (r >= n)
        return -1;
    return r;
}


// n的排列 1-n之间的数各出现恰好一次(没有重复的元素)
// 话费一个金钱将序列交换一次 最终目标将序列变成有序
// 最小交换次数 = 逆序对数
// 冒泡排序交换次数 = 逆序对数

vector<int> seq;        // 原序列   
vector<int> seqTemp;    // 用以辅助计算的临时数组
long long cnt;          // 统计逆序对个数

void mergeSort(int l, int r)
{
    if (l == r)
        return;
    int mid = (l+r) >> 1;
    mergeSort(l, mid);
    mergeSort(mid+1, r);
    int p = l;
    int q = mid+1;
    for (int i = l; i <= r; ++i)
    {
        if (q > r || p <= mid && seq[p] <= seq[q])
            seqTemp[i] = seq[p++];
        else
        {
            seqTemp[i] = seq[q++];
            cnt += mid-p+1;
        }
    }
    for (int i = l; i <= r; ++i)
        seq[i] = seqTemp[i];
}

// n序列长度
// a序列
// 返回值:最少花费的金钱
long long getAnswer(int n, vector<int> &a)
{
    seq = a;
    seqTemp.resize(n);
    cnt = 0;
    mergeSort(0, n-1);
    return cnt;
}


// dijkstra 最短路径

// 优先级更新策略
// Dijkstra by pfs(priority first search)

template<typename Tv, typename Te>  // 顶点类型、边类型
template <typename PU>  // 优先级更新器
void Graph<Tv, Te>::pfs(int s, PU prioUpdater) {    // 优先级更新策略,因算法而异
    priority(s) = 0; status(s) = VISITED; parent(s) = -1;   // 起点s添加到pfs树中
    while (1) { // 将下一顶点和边加至pfs树中,依次引入n-1个顶点和n-1条边
        for (int w = firstNbr(s); -1 < w; w = nextNbr(s, w))    // 对s各邻居w
            prioUpdater(this, s, w);    // 更新顶点的优先级及其父顶点
        for (int shortest = INT_MAX, w = 0; w < n; w++) {
            if (UNDISCOVERED == status(w)) {    // 从尚未加入遍历树的顶点中
                if (shortest > priority(w)) {   // 选出下一个
                    shortest = priority(w); // 优先级最高的顶点s
                    s = w;
                }
            }
        }
        if (VISITED == status(s))   // 直至所有顶点均已加入
            break;
        status(s) = VISITED;
        type(parent(s), s) = TREE;  // 将s加入遍历树
    }
}

template<typename Tv, typename Te>  // 顶点类型 边类型
struct DijkstraPU { // Dijkstra算法的顶点优先级更新器
    virtual void operator()(Graph<Tv, Te>* g, int uk, int v) {  // 对uk的每个 u0 u1 u2...uk uk是最后一个被加进去的点
        if (UNDISCOVERED != g->status(v))   // 尚未被发现的邻居v
            return;
        if (g->priprity(v) > g->priority(uk) + g->weight(uk, v)) {  // 按Dijkstra
            g->priority(v) = g->priority(uk) + g->weight(uk, v);    // 策略
            g->parent(v) = uk;  //  做松弛
        }
    }
};

// 调用
g->pfs(0, DijkstraPU<char, int>());
