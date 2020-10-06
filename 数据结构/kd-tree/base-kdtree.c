
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
 
// Dimensional 维度
#define MAX_DIM 3

struct kd_node_t {
    double point[MAX_DIM];          // 坐标数组 最多支持3维 point=point[0] y=point[1] z=point[2]
    struct kd_node_t *left;
    struct kd_node_t *right;
};

// 两点间距离的平方
// dim 点坐标维度
double dist2(struct kd_node_t *a, struct kd_node_t *b, int dim) {
    double t, d2 = 0;
    while (dim--) {
        t = a->point[dim] - b->point[dim];
        d2 += t * t;
    }
    return d2;
}

void swap(struct kd_node_t *point, struct kd_node_t *y) {
    double tmp[MAX_DIM];
    memcpy(tmp, point->point, sizeof(tmp));
    memcpy(point->point, y->point, sizeof(tmp));
    memcpy(y->point, tmp, sizeof(tmp));
}

// 划分函数得到中间节点(过程中排序)
// idx 点坐标的第几维
struct kd_node_t *find_median(struct kd_node_t *start, struct kd_node_t *end, int idx) {
    if (end <= start) {
        return NULL;
    }
    if (end == start + 1) {
        return start;
    }

    struct kd_node_t *p;
    struct kd_node_t *store;
    struct kd_node_t *md = start + (end - start) / 2;
    double pivot;
    while (1) {
        pivot = md->point[idx];

        swap(md, end-1);
        for (store = p = start; p < end; p++) {
            if (p->point[idx] < pivot) {
                if (p != store) {
                    swap(p, store);
                }
                store++;
            }
        }
        swap(store, end-1);

        if (store->point[idx] == md->point[idx]) {
            return md;
        }

        if (store > md) {
            end = store;
        } else {
            start = store;
        }
    }
}

// 构建树
// t 节点数组
// len 节点数组长度
// i 点坐标第几维
// dim 点坐标维度
struct kd_node_t *make_tree(struct kd_node_t *t, int len, int i, int dim) {
    struct kd_node_t *n;

    if (!len) return 0;

    // 划分一趟得到n点
    if ((n = find_median(t, t+len, i))) {
        // 分别按照维度来划分 例如x y z ... x y z ... ...
        i = (i + 1) % dim;
        // 递归处理n点左半部分
        n->left = make_tree(t, n-t, i, dim);
        // 递归处理n点右半部分
        n->right = make_tree(n+1, t+len-(n+1), i, dim);
    }
    return n;
}

int visited;

// 
// root 树的根节点
// nd 查找的节点
// i 点坐标的第几维
// dim 点坐标维度
// best 结果点
// best_dist2 结果点与查找点的距离平方
void nearest(struct kd_node_t *root, struct kd_node_t *nd, int i, int dim,
        struct kd_node_t **best, double *best_dist2) {
    double d2, dx, dx2;

    // 移动到叶子节点
    if (!root) return;
    // 根节点和查找节点 所有维度的距离平方和
    d2 = dist2(root, nd, dim);
    // 根节点和查找节点 在某一维度距离  此结果可以得出查找节点在根节点的左边还是右边
    dx = root->point[i] - nd->point[i];
    // 根节点和查找节点 在某一维度距离的平方
    dx2 = dx * dx;

    visited++;

    // 当前节点比最佳点 更靠近输入点 则当前点作为新的输入点
    if (!(*best) || d2 < *best_dist2) {
        *best_dist2 = d2;
        *best = root;
    }

    if (!(*best_dist2)) return;

    // 点坐标维度循环
    if (++i >= dim) {
        i = 0;
    }

    // 往左还是往右的决定方法和插入元素的方法一样
    nearest(dx > 0 ? root->left : root->right, nd, i, dim, best, best_dist2);
    // 剪枝 考虑二维点坐标 假如x维度距离已经很远 则y维度距离近必然两点间距离不会近
    if (dx2 >= *best_dist2) return;
    nearest(dx > 0 ? root->right : root->left, nd, i, dim, best, best_dist2);
}

#define N 1000000
#define rand1() (rand() / (double)RAND_MAX)
#define rand_pt(v) { v.point[0] = rand1(); v.point[1] = rand1(); v.point[2] = rand1(); }

int main(int argc, char *argv[]) {
    int i;
    struct kd_node_t wp[] = {
        {{2, 3}}, {{5, 4}}, {{9, 6}}, {{4, 7}}, {{8, 1}}, {{7, 2}}
    };
    struct kd_node_t testNode = {{9, 2}};
    struct kd_node_t *root, *found, *million;
    double best_dist2;

    int node_count = sizeof(wp)/sizeof(wp[1]);
    root = make_tree(wp, node_count, 0, 2);

    visited = 0;
    found = NULL;
    nearest(root, &testNode, 0, 2, &found, &best_dist2);
    printf("k-d-tree node size %d\nsearching for (%g, %g)\n"
            "found (%g, %g) dist %g\nvisited %d nodes\n\n",
            node_count, testNode.point[0], testNode.point[1],
            found->point[0], found->point[1], sqrt(best_dist2), visited);

    ////////////////////////////////////////////////////////////////
    million = (struct kd_node_t *)calloc(N, sizeof(struct kd_node_t));
    srand(time(0));
    for (i = 0; i < N; i++) {
        rand_pt(million[i]);
    }

    root = make_tree(million, N, 0, 3);
    rand_pt(testNode);

    visited = 0;
    found = NULL;
    nearest(root, &testNode, 0, 3, &found, &best_dist2);

    printf("k-d-tree node size:%d\nsearch for (%g, %g, %g)\n"
            "found (%g %g %g) dist %g\nvisited %d nodes\n",
            N, testNode.point[0], testNode.point[1], testNode.point[2],
            found->point[0], found->point[1], found->point[2],
            sqrt(best_dist2), visited);

    ////////////////////////////////////////////////////////////////
    int sum = 0;
    int test_runs = 100000;
    for (i = 0; i < test_runs; i++) {
        found = NULL;
        visited = 0;
        rand_pt(testNode);
        nearest(root, &testNode, 0, 3, &found, &best_dist2);
        sum += visited;
    }
    printf("\nk-d-tree %d node\n"
            "visited %d nodes for %d random finding (%f per lookup)\n",
            N, sum, test_runs, sum/(double)test_runs);

    return 0;
}

// k-d树定义 https://zh.wikipedia.org/wiki/K-d%E6%A0%91
// 代码来源 https://rosettacode.org/wiki/K-d_tree#C
// 编译 gcc kdtree.c -g -o kd -lm
// 应用 https://www.quora.com/What-is-a-kd-tree-and-what-is-it-used-for