
#ifndef _KD_TREE_H_
#define _KD_TREE_H_

struct kdtree;
struct kdres;


// void kd_clear(struct kdtree *tree);
// void kd_data_destructor(struct kdtree *tree, void (*destr)(void *));
// struct kdres *kd_nearest3(struct kd_tree *tree, double x, double y, double z);
// struct kdres *kd_nearest_range3(struct kdtree *tree, double x, double y, double z, double range);

struct kdtree *kd_create(int k);
void kd_free(struct kdtree *tree);

int kd_insert3(struct kdtree *tree, double x, double y, double z, void *data);
struct kdres *kd_nearest_range(struct kdtree *tree, const double *pos, double range);

int kd_res_size(struct kdres *set);
int kd_res_next(struct kdres *set);
int kd_res_end(struct kdres *set);
void kd_res_free(struct kdres *set);
void *kd_res_item(struct kdres *set, double *pos);

#endif