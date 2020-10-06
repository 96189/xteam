
#include "sbd_tree.h"
#include <time.h>
#include <random>
#include <set>
#include <iostream>
#include <chrono>
#include <assert.h>

const int MAX_POINT_NUM = 10000000;
const int RADIUS = 10;

int generate_points(std::set<qtree::Point>& points_set, qtree::Point &time_point) {
    std::default_random_engine random(time(NULL));
    std::uniform_int_distribution<int> x_dis(-40075452, 40075452);
    std::uniform_int_distribution<int> y_dis(-19928981, 19928981);

    int count = 0;
    while (count++ < MAX_POINT_NUM) {
        qtree::Point p;
        p.x = x_dis(random);
        p.y = y_dis(random);
        points_set.insert(p);
        // std::cout << "point -> (" << p.x << "," << p.y << ")" << std::endl;
    }

    // 模拟时光机数据
    auto it = points_set.begin();
    time_point.x = it->x;
    time_point.y = it->y;
    for (int i = 0; i < 5; ++i) {
        qtree::Point p;
        std::uniform_int_distribution<int> dis(-1*RADIUS, RADIUS);
        p.x = time_point.x + dis(random);
        p.y = time_point.y + dis(random);
        points_set.insert(p);
        // std::cout << "make point :" << "(" << p.x << "," << p.y << ")" << std::endl;
    }
    return 0;
}

int make_qtree(const std::set<qtree::Point>& points_set, qtree::QTree& tree) {
    int count = 0;
    for (auto it = points_set.begin(); 
        it != points_set.end();
        ++it) {
        qtree::Point position;
        position.x = it->x;
        position.y = it->y;

        if (tree.AddNode(position, count)) {
            ++count;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    // 随机生成点
    std::set<qtree::Point> points_set;
    qtree::Point time_point;
    generate_points(points_set, time_point);

    // 构建四叉树
    qtree::Bound bound;
	bound.left = -40075452;
	bound.top = -19928981;
	bound.right = 40075452;
	bound.bottom = 19928981;

    auto t1 = std::chrono::steady_clock::now();
    qtree::QTree tree(bound);
    make_qtree(points_set, tree);
    auto t2 = std::chrono::steady_clock::now();
    double dr_ms = std::chrono::duration<double,std::milli>(t2 - t1).count();
    std::cout << "build tree consume :" << dr_ms << "(ms)" << std::endl;

    
    int search_times = 0;
    int count = 0;
    qtree::Bound bound_for_search;
    std::vector<qtree::DataNodeRecord> search_result;
    
    // 划定范围搜索
    search_result.clear();

	bound_for_search.left = -40075452;
	bound_for_search.top = -19928981;
	bound_for_search.right = 40075452;
	bound_for_search.bottom = 19928981;

    t1 = std::chrono::steady_clock::now();
    count = tree.Search(bound_for_search, search_result, search_times);
    assert(count == search_result.size());
    t2 = std::chrono::steady_clock::now();
    dr_ms = std::chrono::duration<double,std::milli>(t2 - t1).count();
    std::cout << count << " search consume :" << dr_ms << " (ms)" << " search_times:" << search_times << std::endl;

    // 根据点坐标画框搜索
    search_times = 0;
    search_result.clear();

    bound_for_search.left = time_point.x - RADIUS;
    bound_for_search.top = time_point.y - RADIUS;
    bound_for_search.right = time_point.x + RADIUS;
    bound_for_search.bottom = time_point.y + RADIUS;
    t1 = std::chrono::steady_clock::now();
    count = tree.Search(bound_for_search, search_result, search_times);
    assert(count == search_result.size());
    t2 = std::chrono::steady_clock::now();
    dr_ms = std::chrono::duration<double,std::milli>(t2 - t1).count();
    std::cout << count << " search consume :" << dr_ms << " (ms)" << " search_times:" << search_times << std::endl;
    for (auto it = search_result.begin(); it != search_result.end(); ++it) {
         it->Print();
    }

    // 最接近点搜索
    std::cout << "search point (" << time_point.x << "," << time_point.y << ")" << std::endl;
    t1 = std::chrono::steady_clock::now();
    int index = tree.SearchForNearestPoint(time_point.x, time_point.y, RADIUS);
    t2 = std::chrono::steady_clock::now();
    dr_ms = std::chrono::duration<double,std::milli>(t2 - t1).count();
    std::cout << "nearest point index: " << index << " consume " << dr_ms << "(ms)" << std::endl;

    return 0;
}