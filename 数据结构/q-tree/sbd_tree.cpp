
#include <stdlib.h>
#include <cstdio>
#include "sbd_tree.h"

const int rect_search_stop_num = 50;
const int rect_search_stop_radius = 1;
const int rect_search_max_radius = 1000;

namespace qtree {
    //四叉树文件头标记
    char FILE_HEAD_MAGIC[] = "QTDP1000";

    bool Point::operator < (const Point& pt) const {
        if (this->x < pt.x) {
            return true;
        } else if (this->x == pt.x && this->y < pt.y) {
            return true;
        } else {
            return false;
        }
    }

    bool Point::operator==(const Point& pt) const {
        return this->x == pt.x && this->y == pt.y;
    }

    //获取两个点的距离平方
    static inline uint64_t distance(qtree::pos_type x1, qtree::pos_type y1, qtree::pos_type x2, qtree::pos_type y2) {
        qtree::pos_type dx = x1 - x2;
        qtree::pos_type dy = y1 - y2;
        return dx * dx + dy * dy;
    }

    void Bound::SetData(pos_type left, pos_type top, pos_type right, pos_type bottom) {
        this->left = left;
        this->top = top;
        this->right = right;
        this->bottom = bottom;
    }

    bool Bound::IsEmpty() const {
        if (left >= right || top >= bottom) {
            return true;
        }

        return false;
    }

    Point Bound::Center() const {
        Point center;
        center.x = left + (right - left) / 2;
        center.y = top + (bottom - top) / 2;
        return center;
    }

    bool Bound::CanDivide() const {
        return ((right - left) / 2 != 0) || ((bottom - top) / 2 != 0);
    }

    bool Bound::Intersect(const Bound& bound) const {
        if (bound.left > right || bound.top > bottom || bound.right < left || bound.bottom < top) {
            return false;
        }

        return true;
    }

    bool Bound::Contain(const Point& point) const {
        if (point.x < left || point.x > right || point.y < top || point.y > bottom) {
            return false;
        }

        return true;
    }

    bool Bound::Contain(const Bound& bound) const {
        if (bound.left < left || bound.top < top || bound.right > right || bound.bottom > bottom) {
            return false;
        }

        return true;
    }

    Node::Node() {
        type = INTER;
        memset(&record, 0, sizeof(record));
        record.inter.bound.SetData(0, 0, 0, 0);
        record.inter.child_count = 0;

        for (int i = 0; i < 4; i++) {
            child[i] = NULL;
        }
    }

    Node::Node(const Bound& bound) {
        type = INTER;
        memset(&record, 0, sizeof(record));
        record.inter.bound = bound;
        record.inter.child_count = 0;

        for (int i = 0; i < 4; i++) {
            child[i] = NULL;
        }
    }

    Node::Node(const Point& position, node_data_type data) {
        type = DATA;
        memset(&record, 0, sizeof(record));
        record.data.position = position;
        record.data.data = data;
        record.data.next = NULL;

        for (int i = 0; i < 4; i++) {
            child[i] = NULL;
        }
    }

    Node::~Node() {
    }

    QTree::QTree(const Bound& bound) {
        root = new Node(bound);
        dump_mode = false;
        be_list = false;
    }

    QTree::QTree(const char* file_name) throw(int) {
        root = NULL;

        printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);
        if (!LoadFromDumpFile(file_name)) {
            printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);
            throw - 1;
            printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);
        }
        printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);
    }

    QTree::~QTree() {
        if (root) {
            if (dump_mode) {
                char* buffer = (char*) root;
                delete[] buffer;
            } else {
                DeleteNode(root);
            }
        }
    }

    bool QTree::AddNode(const Point& position, node_data_type data) {
        Node* node = new Node(position, data);
        bool add_ok = AddNode(root, node);

        if (!add_ok) {
            delete node;
        }

        return add_ok;
    }
    int QTree::SearchForNearestPoint(qtree::pos_type x, qtree::pos_type y, size_t max_radius) {
        this->RectSearchForNearestPoint(x, y, max_radius);
    }
    int QTree::RectSearchForNearestPoint(qtree::pos_type x, qtree::pos_type y,
            size_t max_radius, size_t radius,
            size_t last_radius, int max_empty_radius,
            int min_overwhelm_radius, bool bForceEnd) {
        qtree::Bound query;

        qtree::pos_type ibx = x - radius;
        qtree::pos_type iby = y - radius;
        qtree::pos_type itx = x + radius;
        qtree::pos_type ity = y + radius;

        query.SetData(ibx, iby, itx, ity);
        size_t min_pointcount = (size_t)rect_search_stop_num;
        size_t min_radius = (size_t)rect_search_stop_radius;
        size_t pointcount = GetPointCount(query);

        //printf("radius - %zu, ptcnt - %zu\n", radius, pointcount);
        if (pointcount == 0) {
            //继续递归
            if (radius > max_radius) {
                //printf("not found\n");
                //printf("------------------\n");
                return -1;
            }

            if (max_empty_radius == -1) {
                //还没有开始缩小，半分查找
                return RectSearchForNearestPoint(x, y, max_radius,
                        radius * 2, radius, -1, min_overwhelm_radius);
            } else {
                //已经开始缩小，但是这次找的半径太小，需要扩大，标记minradius
                size_t next_radius = (radius + min_overwhelm_radius) / 2;

                if (next_radius == radius) {
                    //说明半径缩小到1，但还是死循环了(多1m则多了，少1m则为0)
                    return RectSearchForNearestPoint(x, y,
                            max_radius,  min_overwhelm_radius, radius, radius,
                            min_overwhelm_radius, true);
                } else {
                    return RectSearchForNearestPoint(x, y,
                            max_radius, (radius + min_overwhelm_radius) / 2,
                            radius, radius, min_overwhelm_radius);
                }
            }
        } else if (pointcount <= min_pointcount
                || (max_empty_radius != -1 && abs(int(radius) - int(last_radius)) < min_radius)
                || bForceEnd) {
            //本次查找点数足够少，或是已经在往回半分查找，且两次查找的半径太接近
            const double DISTANCE_MAX = 1000000000;
            double des = DISTANCE_MAX;
            std::vector<qtree::DataNodeRecord> rec_res;
            int search_times = 0;
            Search(query, rec_res, search_times);
            size_t point_count = rec_res.size();
            //寻找到的ID
            int findindex = -1;

            for (size_t i = 0; i < point_count; i++) {
                qtree::DataNodeRecord data_node_record = rec_res[i];
                qtree::node_data_type index = data_node_record.data;

                uint64_t tmp = distance(x, y, data_node_record.position.x, data_node_record.position.y);

                if (des >= DISTANCE_MAX || (des < DISTANCE_MAX && tmp < des)) {
                    findindex = index;
                    des = tmp;
                }
            }

            if (findindex == -1 && radius <= max_radius && max_empty_radius == -1) {
                return RectSearchForNearestPoint(x, y, max_radius,
                        radius * 2, radius, max_empty_radius, min_overwhelm_radius);
            } else {
                //printf("------------------\n");
                return findindex;
            }
        } else {
            //数量太大，缩小一点
            if (max_empty_radius == -1) {
                //之前仍在增长
                return RectSearchForNearestPoint(x, y, max_radius,
                        (radius + last_radius) / 2, radius, last_radius, radius);
            } else {
                if (radius < last_radius) {
                    //本次搜索比上次搜索半径要小，但还是点太多，所以需要继续缩小
                    return RectSearchForNearestPoint(x, y,
                            max_radius, (radius + max_empty_radius) / 2,
                            radius, max_empty_radius, radius);
                } else {
                    //本次搜索比上次搜索半径大，说明上次没有点但是这次点多了，半分查找
                    return RectSearchForNearestPoint(x, y,
                            max_radius, (radius + last_radius) / 2, radius, max_empty_radius, radius);
                }

            }

        }
    }

    bool QTree::AddNode(Node* root_node, Node* new_node) {
        Node** child_node = NULL;
        Bound child_bound;
        Bound old_bound = root_node->record.inter.bound;

        if (!old_bound.CanDivide()) {
            return false;
        }

        Point center = old_bound.Center();
        Point new_position = new_node->record.data.position;

        // 插入节点 
        if (new_position.x <= center.x) {
            // 左上区域 确定矩形框范围
            if (new_position.y <= center.y) {
                child_node = &root_node->child[0];
                child_bound.left = old_bound.left;
                child_bound.top = old_bound.top;
                child_bound.right = center.x;
                child_bound.bottom = center.y;
            // 左下区域
            } else {
                child_node = &root_node->child[1];
                child_bound.left = old_bound.left;
                child_bound.top = center.y;
                child_bound.right = center.x;
                child_bound.bottom = old_bound.bottom;
            }
        } else {
            // 右上矩形
            if (new_position.y <= center.y) {
                child_node = &root_node->child[2];
                child_bound.left = center.x;
                child_bound.top = old_bound.top;
                child_bound.right = old_bound.right;
                child_bound.bottom = center.y;
            // 右下矩形
            } else {
                child_node = &root_node->child[3];
                child_bound.left = center.x;
                child_bound.top = center.y;
                child_bound.right = old_bound.right;
                child_bound.bottom = old_bound.bottom;
            }
        }

        // 本矩形框之前没有分裂过
        if (*child_node == NULL) {
            root_node->record.inter.child_count += 1;
            // 置4个子树其中一个为新节点 分裂为新矩形框
            *child_node = new_node;
            return true;
        // 本矩形框之前已经分裂过 且是内部节点  当前点落在子矩形框中 递归向下
        } else if ((*child_node)->type == INTER) {
            bool add_ok = AddNode(*child_node, new_node);

            if (add_ok) {
                root_node->record.inter.child_count += 1;
            }

            return add_ok;
        // 本矩形框之前分裂过 且是数据节点
        } else {
            // 如果坐标相等 取决于是否开启链表模式
            if (new_position.x == (*child_node)->record.data.position.x
                    && new_position.y == (*child_node)->record.data.position.y) {
                if (this->be_list == true) {
                    Node* tmp_node = (*child_node)->record.data.next;
                    (*child_node)->record.data.next = new_node;
                    new_node->record.data.next = tmp_node;
                    root_node->record.inter.child_count += 1;

                    return true;
                } else {
                    //插入失败
                    return false;
                }
            }

            // 子区域矩形可以继续划分
            if (child_bound.CanDivide()) {
                Node* old_node = *child_node;
                // 生成该区域表示的新内部节点
                *child_node = new Node(child_bound);

                // 将原来的数据节点挂到新节点下
                if (!AddNode(*child_node, old_node)) {
                    delete *child_node;
                    *child_node = old_node;

                    return false;
                }

                // 将待插入节点 挂到新节点下
                bool add_ok = AddNode(*child_node, new_node);

                if (add_ok) {
                    // 数据节点变成内部节点 子树个数+1
                    root_node->record.inter.child_count += 1;
                }

                return add_ok;
            } else if (this->be_list) {
                Node* tmp_node = (*child_node)->record.data.next;
                (*child_node)->record.data.next = new_node;
                new_node->record.data.next = tmp_node;
                root_node->record.inter.child_count += 1;

                return true;
            } else {
                //插入失败
                return false;
            }
        }

        return true;
    }

    void QTree::DeleteNode(Node* node) {
        for (int i = 0; i < 4; i++) {
            if (node->child[i]) {
                DeleteNode(node->child[i]);
            }
        }

        if (node->type == DATA) {
            if (this->get_list_mode()) {
                while (node->record.data.next != NULL) {
                    Node* next_node = node->record.data.next;
                    node->record.data.next = next_node->record.data.next;
                    delete next_node;
                }
            }
        }

        delete node;
    }

    int QTree::GetPointCount(const Bound& query) {
        return GetPointCount(root, query);
    }

    int QTree::GetPointCount(Node* root_node, const Bound& query) {
        if (root_node->type == DATA) {
            if (query.Contain(root_node->record.data.position)) {
                int child_count = 1;

                if (this->be_list) {
                    Node* next_node = root_node->record.data.next;

                    while (next_node != NULL) {
                        child_count++;
                        next_node = next_node->record.data.next;
                    }
                }

                return child_count;
                //return 1;
            } else {
                return 0;
            }
        } else {
            if (root_node->record.inter.bound.Intersect(query)) {
                if (query.Contain(root_node->record.inter.bound)) {
                    return root_node->record.inter.child_count;
                } else {
                    int child_count = 0;

                    for (int i = 0; i < 4; i++) {
                        if (root_node->child[i]) {
                            child_count += GetPointCount(root_node->child[i],
                                    query);
                        }
                    }

                    return child_count;
                }
            } else {
                return 0;
            }
        }

    }

    bool QTree::GetPointExistance(const Bound& query) {
        return GetPointExistance(root, query);
    }

    bool QTree::GetPointExistance(Node* root_node, const Bound& query) {
        if (root_node->type == DATA) {
            if (query.Contain(root_node->record.data.position)) {
                return true;
            } else {
                return false;
            }
        } else {
            if (root_node->record.inter.bound.Intersect(query)) {
                if (query.Contain(root_node->record.inter.bound)) {
                    return true;
                } else {
                    for (int i = 0; i < 4; i++) {
                        if (root_node->child[i]) {
                            if (GetPointExistance(root_node->child[i], query)
                                    == true) {
                                return true;
                            }
                        }
                    }

                    return false;
                }
            } else {
                return false;
            }
        }
    }

    int QTree::Search(const Bound& query,
            std::vector<qtree::DataNodeRecord>& result, int &search_times) const {
        result.clear();
        return Search(root, query, result, search_times);
    }

    int QTree::Search(Node* root_node, const Bound& query,
            std::vector<qtree::DataNodeRecord>& result, int &search_times) const {
        if (root_node->type == DATA) {
            if (query.Contain(root_node->record.data.position)) {
                result.push_back(root_node->record.data);

                Node* next_node = root_node->record.data.next;

                while (next_node != NULL) {
                    result.push_back(next_node->record.data);
                    next_node = next_node->record.data.next;
                }
            }
        } else {
            if (root_node->record.inter.bound.Intersect(query)) {
                for (int i = 0; i < 4; i++) {
                    if (root_node->child[i]) {
                        Search(root_node->child[i], query, result, ++search_times);
                    }
                }
            }
        }

        return result.size();
    }

    bool QTree::DumpToFile(const char* file_name) {
        FILE* fp = fopen(file_name, "wb");

        if (fp == NULL) {
            fprintf(stderr, "Fail to open file-%s to write.\n", file_name);
            return false;
        }

        bool ret = DumpToFile(fp);
        fclose(fp);
        return ret;
    }

    bool QTree::DumpToFile(FILE* fp) {
        //设置文件头，用于检验文件类型和版本
        fwrite(FILE_HEAD_MAGIC, 1, strlen(FILE_HEAD_MAGIC), fp);
        return DumpNode(fp, root);
    }

    bool QTree::LoadFromDumpFile(const char* file_name) {
        printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);
        printf("File:%s, Line:%d, Function:%s, File_Name:%s \n", __FILE__, __LINE__, __FUNCTION__, file_name);
        FILE* fp = fopen(file_name, "rb");
        printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);

        if (fp == NULL) {
            printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);
            perror("The following error occurred");
            fprintf(stderr, "Fail to open dump file.\n");
            printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);
            return false;
        }

        printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);
        bool ret = LoadFromDumpFile(fp);
        printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);
        fclose(fp);

        return ret;
    }

    bool QTree::LoadFromDumpFile(FILE* fp) {
        //读取文件头以验证文件类型是否正确
        char head[32];
        int head_len = strlen(FILE_HEAD_MAGIC);
        printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);
        fread(head, 1, head_len, fp);
        head[head_len] = '\0';

        printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);

        if (strcmp(head, FILE_HEAD_MAGIC) != 0) {
            fprintf(stderr, "wrong file type");
            return false;
        }

        if (root) {
            printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);

            if (dump_mode) {
                printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);
                char* buffer = (char*) root;
                delete[] buffer;
            } else {
                printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);
                DeleteNode(root);
            }
        }

        printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);
        size_t cur_pos = ftell(fp);
        printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);
        fseek(fp, 0, SEEK_END);
        printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);
        size_t size = ftell(fp) - cur_pos;
        char* buffer = new char[size];
        fseek(fp, cur_pos, SEEK_SET);
        printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);
        fread(buffer, 1, size, fp);
        size_t offset = 0;
        printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);
        root = LoadNodeFromBuffer(buffer, offset);
        printf("File:%s, Line:%d, Function:%s \n", __FILE__, __LINE__, __FUNCTION__);
        dump_mode = true;

        return true;
    }

    bool QTree::DumpNode(FILE* fp, Node* node) {
        Node temp_node = *node;

        for (int i = 0; i < 4; i++) {
            //此处其实可以省略，但是这样会导致同一棵树每次写出的文件md5不同
            if (temp_node.child[i]) {
                temp_node.child[i] = (Node*) 0x1;
            }
        }

        fwrite(&temp_node, 1, sizeof(Node), fp);

        if (node->type == DATA) {
            Node* next_node = node->record.data.next;

            while (next_node != NULL) {
                temp_node = *next_node;

                if (temp_node.record.data.next) {
                    temp_node.record.data.next = (Node*)0x1;
                }

                fwrite(&temp_node, 1, sizeof(Node), fp);
                next_node = next_node->record.data.next;
            }
        }

        for (int i = 0; i < 4; i++) {
            if (node->child[i]) {
                DumpNode(fp, node->child[i]);
            }
        }

        return true;
    }

    Node* QTree::LoadNodeFromBuffer(const char* buffer, size_t& offset) {
        Node* node = (Node*)(buffer + offset);
        offset += sizeof(Node);

        if (node->type == DATA) {
            Node* tmp_node = node;

            while (tmp_node->record.data.next != NULL) {
                if (be_list == false) {
                    be_list = true;
                }

                tmp_node->record.data.next = (Node*)(buffer + offset);
                offset += sizeof(Node);
                tmp_node = tmp_node->record.data.next;
            }
        }

        for (int i = 0; i < 4; i++) {
            if (node->child[i]) {
                node->child[i] = LoadNodeFromBuffer(buffer, offset);
            }
        }

        return node;
    }

    void QTree::set_list_mode(bool be_list) {
        this->be_list = be_list;
    }

    bool QTree::get_list_mode() {
        return this->be_list;
    }
}
;

