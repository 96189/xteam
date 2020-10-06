
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "kdtree.h"

static double dist_sq(double *a1, double *a2, int dims) {
    double dist_sq = 0, diff;
    while (--dims >= 0) {
        diff = (a1[dims] - a2[dims]);
        dist_sq += diff * diff;
    }
    return dist_sq;
}

static double rd( void ) {
    return (double)rand() / RAND_MAX * 20.0 - 10.0;
}

int main(int argc, char *argv[]) {
    int i = 0;
    int num_pts = 5;

    void *ptree = NULL;
    char *data = NULL;
    char *pch = NULL;

    struct kdres *presults;

    double pos[3];
    double dist;
    // 目标点
    double pt[3] = {0, 0, 1};
    double radius = 10;

    if (!(data = malloc(num_pts * sizeof(char)))) {
        perror("malloc failed");
        return 1;
    }

    srand(time(0));
    ptree = kd_create(3);

    for (i = 0; i < num_pts; ++i) {
        data[i] = 'a' + i;
        int ret = kd_insert3(ptree, rd(), rd(), rd(), &data[i]);
        assert(0 == ret);
    }

    presults = kd_nearest_range(ptree, pt, radius);
    printf("found %d result\n", kd_res_size(presults));

    while (!kd_res_end(presults)) {
        pch = (char*)kd_res_item(presults, pos);
        dist = sqrt(dist_sq(pt, pos, 3));
        printf("node at (%3.f, %3.f, %3.f) away and has data:%c\n",
            pos[0], pos[1], pos[2], dist, *pch);
        kd_res_next(presults);
    }

    free(data);
    kd_res_free(presults);
    kd_free(ptree);
    return 0;
}