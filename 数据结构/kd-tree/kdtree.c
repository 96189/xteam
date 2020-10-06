
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "kdtree.h"

#define SQ(x)   ((x) * (x))

struct kdnode {
    double *pos;
    int idx;
    void *data;
    struct kdnode *left;
    struct kdnode *right;
};

struct kdhyperrect {
    int dim;
    double *min;
    double *max;
};

struct res_node {
    struct kdnode *item;
    double dist_sq;
    struct res_node *next;
};

struct kdtree {
    int dim;
    struct kdnode *root;
    struct kdhyperrect *rect;
    void (*destr)(void *);
};

struct kdres {
    struct kdtree *tree;
    struct res_node *rlist;
    struct res_node *riter;
    int size;
};



static void clear_rec(struct kdnode *node, void (*destr)(void*)) {
	if(!node) return;

	clear_rec(node->left, destr);
	clear_rec(node->right, destr);
	
	if(destr) {
		destr(node->data);
	}
	free(node->pos);
	free(node);
}
static void hyperrect_free(struct kdhyperrect *rect) {
	free(rect->min);
	free(rect->max);
	free(rect);
}
static void kd_clear(struct kdtree *tree) {
	clear_rec(tree->root, tree->destr);
	tree->root = 0;

	if (tree->rect) {
		hyperrect_free(tree->rect);
		tree->rect = 0;
	}
}
static struct kdhyperrect* hyperrect_create(int dim, const double *min, const double *max){
	size_t size = dim * sizeof(double);
	struct kdhyperrect* rect = 0;

	if (!(rect = malloc(sizeof(struct kdhyperrect)))) {
		return 0;
	}

	rect->dim = dim;
	if (!(rect->min = malloc(size))) {
		free(rect);
		return 0;
	}
	if (!(rect->max = malloc(size))) {
		free(rect->min);
		free(rect);
		return 0;
	}
	memcpy(rect->min, min, size);
	memcpy(rect->max, max, size);

	return rect;
}

static void hyperrect_extend(struct kdhyperrect *rect, const double *pos){
	int i;

	for (i=0; i < rect->dim; i++) {
		if (pos[i] < rect->min[i]) {
			rect->min[i] = pos[i];
		}
		if (pos[i] > rect->max[i]) {
			rect->max[i] = pos[i];
		}
	}
}

static struct res_node *free_nodes;
static struct res_node *alloc_resnode(void)
{
	struct res_node *node;

	if(!free_nodes) {
		node = malloc(sizeof *node);
	} else {
		node = free_nodes;
		free_nodes = free_nodes->next;
		node->next = 0;
	}

	return node;
}

static void free_resnode(struct res_node *node){
	node->next = free_nodes;
	free_nodes = node;
}

static int rlist_insert(struct res_node *list, struct kdnode *item, double dist_sq) {
	struct res_node *rnode;

	if(!(rnode = alloc_resnode())) {
		return -1;
	}
	rnode->item = item;
	rnode->dist_sq = dist_sq;

	if(dist_sq >= 0.0) {
		while(list->next && list->next->dist_sq < dist_sq) {
			list = list->next;
		}
	}
	rnode->next = list->next;
	list->next = rnode;
	return 0;
}

int kd_res_size(struct kdres *set) {
    return (set->size);
}
int kd_res_next(struct kdres *rset) {
	rset->riter = rset->riter->next;
	return rset->riter != NULL;
}
int kd_res_end(struct kdres *rset) {
    return rset->riter == NULL;
}

void *kd_res_item(struct kdres *rset, double *pos) {
	if(rset->riter) {
		if(pos) {
			memcpy(pos, rset->riter->item->pos, rset->tree->dim * sizeof *pos);
		}
		return rset->riter->item->data;
	}
	return NULL;
}
void kd_res_rewind(struct kdres *rset)
{
	rset->riter = rset->rlist->next;
}
void kd_free(struct kdtree *tree) {
    if (tree) {
        kd_clear(tree);
        free(tree);
    }
}

static void clear_results(struct kdres *rset) {
	struct res_node *tmp, *node = rset->rlist->next;

	while(node) {
		tmp = node;
		node = node->next;
		free_resnode(tmp);
	}

	rset->rlist->next = 0;
}
void kd_res_free(struct kdres *rset) {
	clear_results(rset);
	free_resnode(rset->rlist);
	free(rset);
}
////////////////////////////////////////////////////////////

struct kdtree *kd_create(int k) {
    struct kdtree *tree;
    if(!(tree = malloc(sizeof(struct kdtree)))) {
        return NULL;
    }

    tree->dim = k;
    tree->root = NULL;
    tree->destr = NULL;
    tree->rect = NULL;

    return tree;
}

static int insert_rec(struct kdnode **nptr, const double *pos, void *data, int idx, int dim) {
	int new_dir;
	struct kdnode *node;

	if(!*nptr) {
		if(!(node = malloc(sizeof *node))) {
			return -1;
		}
		if(!(node->pos = malloc(dim * sizeof *node->pos))) {
			free(node);
			return -1;
		}
		memcpy(node->pos, pos, dim * sizeof *node->pos);
		node->data = data;
		node->idx = idx;
		node->left = node->right = 0;
		*nptr = node;
		return 0;
	}

	node = *nptr;
	new_dir = (node->idx + 1) % dim;
	if(pos[node->idx] < node->pos[node->idx]) {
		return insert_rec(&(*nptr)->left, pos, data, new_dir, dim);
	}
	return insert_rec(&(*nptr)->right, pos, data, new_dir, dim);
}

// 节点插入
int kd_insert(struct kdtree *tree, const double *pos, void *data) {
    if (insert_rec(&tree->root, pos, data, 0, tree->dim)) {
        return -1;
    }

    if (tree->rect == NULL) {
        tree->rect = hyperrect_create(tree->dim, pos, pos);
    } else {
        hyperrect_extend(tree->rect, pos);
    }
    return 0;
}


int kd_insert3(struct kdtree *tree, double x, double y, double z, void *data) {
    double buf[3];
    buf[0] = x;
    buf[1] = y;
    buf[2] = z;
    return kd_insert(tree, buf, data);
}

// 找最近的点
static int find_nearest(struct kdnode *node, const double *pos, double range, struct res_node *list, int ordered, int dim) {
    double dist_sq = 0;
    double dx = 0;
    int i = 0;
    int ret = 0;
    int added_res = 0;

    if (!node) return 0;

    for (i = 0; i < dim; ++i) {
        dist_sq += SQ(node->pos[i] - pos[i]);
    }
    if (dist_sq <= SQ(range)) {
        if (rlist_insert(list, node, ordered ? dist_sq : -1.0) == -1) {
            return -1;
        }
        added_res = 1;
    }

    // TODO
    dx = pos[node->idx] - node->pos[node->idx];

    ret = find_nearest(dx <= 0.0 ? node->left : node->right, pos, range, list, ordered, dim);
    if (ret >= 0 && fabs(dx) < range) {
        added_res += ret;
        ret = find_nearest(dx <= 0.0 ? node->right : node->left, pos, range, list, ordered, dim);
    }
    if (ret == -1) {
        return -1;
    }
    added_res += ret;
    return added_res;
}

// 找区域内的点
struct kdres *kd_nearest_range(struct kdtree *kd, const double *pos, double range) {
    int ret;
    struct kdres *rset;

    if (!(rset = malloc(sizeof(struct kdres)))) {
        return NULL;
    }
    if (!(rset->rlist = alloc_resnode())) {
        free(rset);
        return 0;
    }

    rset->rlist->next = NULL;
    rset->tree = kd;

    if ((ret = find_nearest(kd->root, pos, range, rset->rlist, 0, kd->dim)) == -1) {
        kd_res_free(rset);
        return 0;
    }
    rset->size = ret;
    kd_res_rewind(rset);
    return rset;
}
